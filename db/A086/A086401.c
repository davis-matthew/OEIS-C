/*
 * A086401  a(n) = M(n) (mod n) where M(n) = A001006(n) is the n-th Motzkin
 *          number.
 * https://oeis.org/A086401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = M(n) mod n Where M(n) is the Motzkin number (A001006).
 *            Computation uses the summation M(n) = Σ_{k=0}^{⌊n/2⌋}
 *            binom(n,2k) * Catalan(k), with Catalan(k)=binom(2k,k)/(k+1). The
 *            result is reduced modulo n. All intermediate binomial
 *            computations are performed modulo (n·(k+1)) to keep numbers
 *            wi...
 * Range:     u8 n <= 272, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ---------------------------------------------------------------------- */
/* Utility: integer exponent of prime p in n!                           */
static inline uint64_t exp_in_fact(uint64_t n, uint64_t p)
{
    uint64_t e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* ---------------------------------------------------------------------- */
/* Modular exponentiation (base^exp mod mod) using 128‑bit intermediate  */
static inline uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = (unsigned __int128)result * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------------------------------------------------------------------- */
/* Simple sieve to generate all primes up to 20000 (enough for n≤10000)   */
#define PRIME_SIEVE_LIMIT 20000
static int primes[PRIME_SIEVE_LIMIT + 1];
static int prime_cnt = 0;
static int primes_ready = 0;

static void init_primes(void)
{
    if (primes_ready) return;
    int limit = PRIME_SIEVE_LIMIT;
    static uint8_t is_prime[PRIME_SIEVE_LIMIT + 1];
    for (int i = 2; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }
    for (int i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[prime_cnt++] = i;
    primes_ready = 1;
}

/* ---------------------------------------------------------------------- */
/* binom_mod_small(a,b,mod) returns C(a,b) mod mod (mod fits in 64 bits) */
/* Uses prime factorisation of a!, b!, (a‑b)!                                 */
static uint64_t binom_mod_small(uint32_t a, uint32_t b, uint64_t mod)
{
    if (b > a) return 0;
    if (b > a - b) b = a - b;
    init_primes();
    uint64_t res = 1 % mod;
    for (int i = 0; i < prime_cnt && primes[i] <= a; ++i) {
        uint64_t p = (uint64_t)primes[i];
        uint64_t e = exp_in_fact(a, p) - exp_in_fact(b, p) - exp_in_fact(a - b, p);
        if (e == 0) continue;
        uint64_t term = powmod64(p, e, mod);
        res = (unsigned __int128)res * term % mod;
    }
    return res;
}

/* ---------------------------------------------------------------------- */
/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but safe */
    uint64_t N = (uint64_t)n;
    uint64_t result = 0;
    uint32_t maxk = (uint32_t)(n >> 1);   /* floor(n/2) */

    for (uint32_t k = 0; k <= maxk; ++k) {
        uint64_t denom = (uint64_t)(k + 1);
        uint64_t mod     = (unsigned __int128)N * denom;  /* n*(k+1) */
        uint64_t bin1    = binom_mod_small((uint32_t)n, 2 * k, mod);  /* C(n,2k) */
        uint64_t bin2    = binom_mod_small(2 * k, k, mod);           /* C(2k,k) */
        uint64_t num     = (unsigned __int128)bin1 * bin2 % mod;
        uint64_t term    = num / denom;     /* exact division */
        result += term;
        result %= N;                       /* keep the partial sum small */
    }
    return (seq_t)result;
}
