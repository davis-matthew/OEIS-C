/*
 * A061868  Numbers n such that n^2 has property that the sum of its digits
 *          and the product of its digits are squares (allowing zeros).
 * https://oeis.org/A061868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 920 (2026-11-01)
 * Helpers:   digit_product, digit_sum, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A061868:
 * a(n) = the n‑th integer n such that n^2 has the property that
 * the sum of its decimal digits and the product of its decimal digits
 * are perfect squares (allowing zero).  The function enumerates
 * integers in increasing order, counts those that satisfy the
 * property, and returns the n‑th one.  This is a brute‑force search
 * implementation rather than a closed formula. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/digit_sum.h"
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* n is 1‑based index */
    idx_t count = 0;     /* how many terms found so far */
    idx_t x = 0;         /* current candidate integer */

    while (count < n) {
        seq_t sq = (seq_t)x * (seq_t)x;
        seq_t sum = digit_sum(sq);
        seq_t prod = digit_product(sq);

        seq_t sroot = floor_sqrt(sum);
        if (sroot * sroot != sum) {
            ++x;
            continue;
        }

        seq_t proot = floor_sqrt(prod);
        if (proot * proot != prod) {
            ++x;
            continue;
        }

        ++count;
        if (count == n) {
            return (seq_t)x;
        }
        ++x;
    }

    /* unreachable for n >= 1 */
    return (seq_t)0;
}
