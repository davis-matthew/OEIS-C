/*
 * A043296  Sum of digits of denominator of Bernoulli number B(2n).
 * https://oeis.org/A043296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = digit_sum( ∏{p prime | (p-1) | 2n} p )
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   build_primes, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes.h"
#include "../utils/digit_sum.h"

/* Cached list of primes up to a given limit */
static idx_t *prime_list = NULL;
static size_t prime_cnt  = 0;
static idx_t prime_limit = 0;

/* Ensure that prime_list contains all primes ≤ limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;

    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    build_primes(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* --------------------------------------------------------------- */
/*  a(n):  sum of decimal digits of the denominator of B_{2n}     */
/* --------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* n ≥ 1 as per OEIS offset */
    idx_t m = n << 1;          /* m = 2 * n */
    idx_t limit = m + 1;       /* primes are needed up to 2n+1 */

    ensure_primes(limit);

    seq_t prod = 1;            /* product of qualifying primes */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (m % (p - 1) == 0) {
            prod = prod * (seq_t)p;
        }
    }

    return digit_sum(prod);
}
