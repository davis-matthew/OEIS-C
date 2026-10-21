/*
 * A078315  Minimum exponent in prime factorization of n*rad(n)+1, where rad =
 *          A007947 (the radical or squarefree kernel).
 * https://oeis.org/A078315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum exponent appearing in the prime factorisation of
 *            m = n * rad(n) + 1, where rad(n) is the product of the distinct
 *            prime factors of n. If m is square‑free the minimum exponent is
 *            1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* --- prime sieve (single global initialisation) --- */
#define SIEVE_LIMIT 20000   /* > sqrt(10000^2 + 1) */
static int primes[SIEVE_LIMIT];
static int prime_cnt = 0;
static char is_comp[SIEVE_LIMIT + 1];
static int primes_inited = 0;

static void init_primes(void)
{
    if (primes_inited)
        return;
    for (int i = 2; i <= SIEVE_LIMIT; ++i)
        is_comp[i] = 0;
    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            primes[prime_cnt++] = i;
            if ((long long)i * i <= SIEVE_LIMIT) {
                for (int j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    primes_inited = 1;
}

/* --- compute rad(n) as product of distinct primes dividing n --- */
static uint64_t rad_of(idx_t n)
{
    uint64_t rad = 1;
    uint64_t tmp = n;
    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            rad *= p;           /* multiply by distinct prime once */
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)                  /* remaining prime factor */
        rad *= tmp;
    return rad;
}

/* --- main sequence function --- */
seq_t a(idx_t n)
{
    if (n == 0)                  /* safety: seq offset starts at 1 */
        return 0;

    init_primes();

    /* compute m = n * rad(n) + 1 */
    uint64_t rad_n = rad_of(n);
    uint64_t m = (uint64_t)n * rad_n + 1ULL;

    /* factorize m and find the minimum exponent */
    int min_exp = INT_MAX;
    uint64_t tmp = m;

    for (int i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint64_t p = primes[i];
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp < min_exp)
                min_exp = exp;
            if (min_exp == 1)      /* cannot get smaller */
                return (seq_t)1;
        }
    }
    if (tmp > 1)                  /* remaining prime factor */
        return (seq_t)1;          /* exponent is 1 */

    /* tmp==1 means all factors processed, min_exp holds the minimum */
    return (seq_t)min_exp;
}
