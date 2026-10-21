/*
 * A094661  Numerator of the sum of the ratios of consecutive primes.
 * https://oeis.org/A094661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, changed
 * Formula:   a(n) = numerator( ∑_{k=1..n} prime(k+1)/prime(k) )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   gcd_f
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_f.h"

/* static storage for primes -------------------------------------------- */
static bool primes_prepared = false;
static size_t primes_cnt    = 0;
static uint32_t *primes     = NULL;

/* prepare first ~20000 primes (enough for n ≤ 10000) */
static void prepare_primes(void)
{
    if (primes_prepared)
        return;

    const size_t limit = 200000;                 /* > 104729 = 10001st prime */
    bool *is_prime      = (bool*)malloc(limit + 1);
    if (!is_prime)
        return;
    for (size_t i = 0; i <= limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t q = p * p; q <= limit; q += p)
                is_prime[q] = false;
        }
    }
    /* count primes */
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++primes_cnt;

    primes = (uint32_t*)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) {
        free(is_prime);
        return;
    }
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[idx++] = (uint32_t)i;
    free(is_prime);
    primes_prepared = true;
}

/* ------------------------------------------------------------ */
/* a(idx_t n) : numerator of sum of ratios of consecutive primes */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe */

    prepare_primes();              /* ensures primes array is ready */

    /* we need n+1 primes */
    if (primes_cnt < (size_t)(n + 1))
        return (seq_t)0;          /* insufficient data, should not happen */

    seq_t num = (seq_t)0;  /* numerator of current sum */
    seq_t den = (seq_t)1;  /* denominator of current sum */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t a = (seq_t)primes[(size_t)k - 1];  /* prime_k   */
        seq_t b = (seq_t)primes[(size_t)k];      /* prime_{k+1} */

        /* add b/a to num/den */
        seq_t new_num = num * a + b * den;
        seq_t new_den = den * a;

        seq_t g = gcd_f(new_num, new_den);
        num = new_num / g;
        den = new_den / g;
    }
    return num;
}
