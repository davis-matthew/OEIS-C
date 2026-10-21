/*
 * A073908  Smallest number m such that m and the product of digits of m are
 *          both divisible by 7n, or 0 if no such number exists.
 * https://oeis.org/A073908
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 27, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   digit_product, n_is_invalid
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A073908: a(n) = smallest m such that m and the product of digits of m are
   both divisible by 7n, or 0 if no such number exists.
   Formula: a(n) = A085124(7*n) */

#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/n_is_invalid.h"

static bool has_zero_digit(seq_t x)
{
    seq_t t = x;
    while (t > 0) {
        if ((t % 10) == 0)
            return true;
        t /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* n must be >= 1 */
    if (n < 1)
        return 0;

    if (n_is_invalid(n))
        return 0;

    seq_t d = (seq_t)n * 7;   /* divisor 7n */

    /* search for smallest multiple of d whose digit product is also multiple of d */
    /* limit search to a reasonable number of candidates */
    const unsigned long long MAX_K = 500000ULL;  /* adjust if necessary */
    for (unsigned long long k = 1; k <= MAX_K; ++k) {
        seq_t m = d * (seq_t)k;
        /* skip numbers containing digit 0 – product would be 0, not acceptable */
        if (has_zero_digit(m))
            continue;
        seq_t prod = digit_product(m);
        if ((prod % d) == 0)
            return m;
    }
    return 0;   /* no suitable m found within search bounds */
}
