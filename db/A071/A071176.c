/*
 * A071176  Smallest k such that the concatenation of n and k is a square
 *          (decimal notation).
 * https://oeis.org/A071176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice, look
 * Formula:   a(n) = the smallest k such that the decimal concatenation of n
 *            and k is a perfect square
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/floor_sqrt.h"
#include "../utils/digits.h"

/* Custom concatenation that treats k = 0 as having one decimal digit */
static inline seq_t concat_with_zero(idx_t n, uint64_t k)
{
    int d = (k == 0) ? 1 : digits((idx_t)k);   /* number of decimal digits of k */
    seq_t pow10d = pow10((idx_t)d);            /* 10^d as seq_t */
    return (seq_t)n * pow10d + (seq_t)k;
}

seq_t a(idx_t n)
{
    /* Search for the smallest k >= 0 such that concat(n,k) is a square. */
    for (uint64_t k = 0; ; ++k) {
        seq_t val = concat_with_zero(n, k);
        seq_t r   = floor_sqrt(val);
        if (r * r == val)
            return (seq_t)k;
    }
}
