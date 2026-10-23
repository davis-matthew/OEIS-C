/*
 * A116933  Smallest k greater than 0 such that n+k*p is a prime, where p is
 *          the smallest coprime greater than n+1.
 * https://oeis.org/A116933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that n + k·p is prime, where p =
 *            min{p > n+1 : gcd(n, p) = 1} (p may be composite)
 * Range:     u8 n <= 26, u16 n <= 4761, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Euclidean GCD for seq_t ---------- */
static inline seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* ---------- small miller-rabin / trial divisions for 64‑bit ------- */
#define MAX_SIEVE 200000
static char sieve_flag[MAX_SIEVE + 1];
static uint32_t primes[20000];
static size_t prime_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    for (int i = 2; i <= MAX_SIEVE; ++i) sieve_flag[i] = 1;
    for (int i = 2; i * i <= MAX_SIEVE; ++i) {
        if (sieve_flag[i]) {
            for (int j = i * i; j <= MAX_SIEVE; j += i)
                sieve_flag[j] = 0;
        }
    }
    prime_cnt = 0;
    for (int i = 2; i <= MAX_SIEVE; ++i) {
        if (sieve_flag[i]) {
            if (prime_cnt < 20000)
                primes[prime_cnt++] = (uint32_t)i;
        }
    }
    primes_ready = true;
}

/* integer square root for uint64_t, result <= sqrt(x) */
static uint64_t isqrt_u64(uint64_t x)
{
    uint64_t r = 0;
    while (1) {
        uint64_t r2 = r + 1;
        if (r2 > x / r2) break;   /* avoid overflow */
        r = r2;
    }
    return r;
}

/* primality test using the precomputed primes list */
static bool is_prime_seq(seq_t v)
{
    uint64_t n = (uint64_t)v;        /* values of interest fit in 64 bits */
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    uint64_t r = isqrt_u64(n);
    for (size_t i = 1; i < prime_cnt; ++i) {   /* primes[0]==2 already handled */
        uint32_t p = primes[i];
        if (p > r) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* ---------- sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* out of range, not used */

    init_primes();                       /* ensure prime list ready */

    /* find p > n+1 with gcd(n, p) == 1 */
    uint64_t p = (uint64_t)n + 2;        /* start just after n+1 */
    while (1) {
        if (seq_gcd((seq_t)n, (seq_t)p) == 1) break;
        ++p;
    }

    seq_t base = (seq_t)n;

    for (uint64_t k = 1; ; ++k) {
        seq_t val = base + (seq_t)k * (seq_t)p;
        if (is_prime_seq(val))
            return (seq_t)k;
    }
}
