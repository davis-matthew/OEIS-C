/*
 * A078552  Largest prime dividing sigma(3,n).
 * https://oeis.org/A078552
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime factor of σ₃(n) where σ₃(n) = Σ_{d|n} d³
 *            (sum of cubes of the divisors of n)
 * Range:     u8 n <= 8, u16 n <= 120, u32 n <= 4488, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* -------------  global data  ------------- */
#define MAX_N      100010   /* enough for n up to ~10⁴ */
#define MAX_SIEVE  1000005   /* primes up to 10⁶ for factorising σ₃ */

/* smallest prime factor for all numbers ≤ MAX_N */
static int spf[MAX_N];
/* list of all primes ≤ MAX_SIEVE */
static int primes[MAX_SIEVE];
static int prime_cnt = 0;

/* -------------  initialisation helpers  ------------- */
static void init_sieve(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;

    /* sieve for spf up to MAX_N */
    for (int i = 0; i < MAX_N; ++i) spf[i] = i;
    for (int i = 2; (uint64_t)i * i < (uint64_t)MAX_N; ++i) {
        if (spf[i] == i) {
            for (int j = i * i; j < MAX_N; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    /* sieve for primes up to MAX_SIEVE */
    char is_prime[MAX_SIEVE];
    for (int i = 0; i < MAX_SIEVE; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (int i = 2; (uint64_t)i * i < (uint64_t)MAX_SIEVE; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j < MAX_SIEVE; j += i) is_prime[j] = 0;
        }
    }
    for (int i = 2; i < MAX_SIEVE; ++i) {
        if (is_prime[i]) primes[prime_cnt++] = i;
    }
}

/* -------------  helper to factor n using spf  ------------- */
static void factor_n(uint64_t n, uint64_t *p, uint64_t *e, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint64_t q = spf[n];
        size_t idx = *cnt;
        p[idx] = q;
        e[idx] = 0;
        while (n % q == 0) {
            n /= q;
            ++e[idx];
        }
        ++(*cnt);
    }
}

/* -------------  helper to compute σ₃(n)  ------------- */
static uint64_t sigma3(uint64_t n)
{
    uint64_t p[32], e[32];
    size_t cnt;
    factor_n(n, p, e, &cnt);

    __uint128_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t pp = p[i];
        /* compute sum_{k=0}^{e[i]} pp^{3k} */
        uint64_t term = 1;
        uint64_t power = 1;
        uint64_t p3 = pp * pp * pp;   /* pp^3, fits in 64 bit (pp ≤ 10⁴) */
        for (uint64_t kk = 0; kk < e[i]; ++kk) {
            power = (__uint128_t)power * p3;
            term += (uint64_t)power;
        }
        result = result * term;
    }
    return (uint64_t)result;
}

/* -------------  helper to get largest prime factor of x  ------------- */
static uint64_t largest_prime_factor(uint64_t x)
{
    uint64_t largest = 1;
    uint64_t tmp = x;
    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            largest = p;
            do { tmp /= p; } while (tmp % p == 0);
        }
    }
    if (tmp > 1) largest = tmp;
    return largest;
}

/* -------------  main sequence function  ------------- */
seq_t a(idx_t n)
{
    /* initialisation on first use */
    static int first = 1;
    if (first) { init_sieve(); first = 0; }

    if (n < 2) return 0;          /* out of defined range – behaviour undefined */

    uint64_t s3 = sigma3((uint64_t)n);
    uint64_t lpf = largest_prime_factor(s3);
    return (seq_t)lpf;
}
