/*
 * A068671  Define f(n,k) = floor[n/k] and a(n) = right concatenation of
 *          f(n,k) for k = 1 to int[(n+1)/2] or until one arrives at a 1.
 * https://oeis.org/A068671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = right concatenation of floor(n/k) for k = 1,2,... until
 *            floor(n/k) = 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow10_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10_seq.h"

/* Multiply by 10^e. */

static inline idx_t digit_count_idx(idx_t v)
{
    idx_t c = 1;
    while (v >= 10) {
        v /= 10;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 1;; ++k) {
        idx_t q = n / k;
        idx_t d = digit_count_idx(q);
        res = res * pow10_seq(d) + (seq_t)q;
        if (q == 1)
            break;
    }
    return res;
}
