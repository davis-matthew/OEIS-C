/*
 * A058355  Coefficients in the series (1 + x^2 + x^3 + x^5 + x^7 + x^11 +
 *          x^13 + ... )/(1 - x - x^4 - x^6 - x^8 - x^9 - x^10 - x^12 - x^14 -
 *          ... ).
 * https://oeis.org/A058355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Σ_{c composite, 4 ≤ c ≤ n} a(n-c) + (n == 0 or n
 *            is prime ? 1 : 0)
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* -------------  implementation follows  ------------- */
#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;          /* stored values a(0)..a(k) */
static idx_t a_cache_size = 0;         /* number of stored values */

static uint8_t *prime_flag = NULL;     /* 1 if the index is prime, 0 otherwise */
static idx_t prime_flag_size = 0;      /* size of prime_flag array */

/* resize the prime_flag array and sieve primes up to at least n */
static void ensure_prime_flag(idx_t n)
{
    if (n < prime_flag_size)
        return;

    uint8_t *new_flag = (uint8_t *)realloc(prime_flag, (size_t)(n + 1));
    if (!new_flag)
        return;          /* out of memory – caller will fail */
    prime_flag = new_flag;

    /* initial assumption: all numbers are prime */
    for (idx_t i = 0; i <= n; ++i)
        prime_flag[i] = 1;
    prime_flag[0] = 0;
    if (n >= 1)
        prime_flag[1] = 0;

    /* simple sieve */
    for (idx_t i = 2; i * i <= n; ++i) {
        if (prime_flag[i]) {
            for (idx_t j = i * i; j <= n; j += i)
                prime_flag[j] = 0;
        }
    }
}

/* compute a(i) for all i from a_cache_size to n and store them */
static void ensure_cache(idx_t n)
{
    if (n < a_cache_size)
        return;

    seq_t *new_cache = (seq_t *)realloc(a_cache,
            (size_t)(n + 1) * sizeof(seq_t));
    if (!new_cache)
        return;          /* out of memory – caller will fail */
    a_cache = new_cache;
    idx_t old_size = a_cache_size;
    a_cache_size = n + 1;

    /* need prime info for the new range */
    ensure_prime_flag(n);

    for (idx_t i = old_size; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            continue;
        }

        seq_t sum = a_cache[i - 1];  /* a(i-1) term */

        /* add contributions from composite gaps m ≥ 4 */
        for (idx_t m = 4; m <= i; ++m) {
            if (!prime_flag[m])           /* m composite */
                sum += a_cache[i - m];
        }

        /* add 1 if i is prime (i==0 already handled) */
        if (prime_flag[i])
            sum += (seq_t)1;

        a_cache[i] = sum;
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
