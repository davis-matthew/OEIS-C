/*
 * A137804  a(n) = floor(n*(4*sqrt(2)+9)/7).
 * https://oeis.org/A137804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(4*sqrt(2)+9)/7)
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bitlen_seq_t.h"

/* integer square root: floor(sqrt(x)) for arbitrary integer type seq_t */
static inline seq_t isqrt_seq(seq_t x)
{
    if (x == 0) return 0;
    unsigned bits = bitlen_seq_t(x);
    seq_t low  = 0;
    seq_t high = ((seq_t)1) << ((bits + 1) / 2); /* 2^(ceil(bits/2)) is an upper bound */

    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t mid_sq = mid * mid;
        if (mid_sq <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t N = (seq_t)n;

    /* A = 9*n, B = 4*n */
    seq_t A = 9 * N;
    seq_t B = 4 * N;

    /* left = B^2 * 2  (used for comparison) */
    seq_t left = B * B * 2;

    /* b = floor(B*sqrt(2)) = floor(sqrt(B^2 * 2)) */
    seq_t b = isqrt_seq(left);

    /* base = A + b  (integer part of A + B*sqrt(2)) */
    seq_t base = A + b;

    /* q0 = floor(base / 7) */
    seq_t q0 = base / 7;

    /* m_value = (q0 + 1) * 7 - A  (value to test for possible +1) */
    seq_t m_value = (q0 + 1) * 7 - A;

    /* Determine if we need to add 1 */
    int add_one = 0;
    if (m_value <= 0) {
        /* RHS of inequality is negative or zero, so inequality holds */
        add_one = 1;
    } else {
        /* Inequality: B^2 * 2 >= m_value^2
           Avoid overflow by dividing: left / m_value >= m_value */
        seq_t div = left / m_value;
        if (div >= m_value) {
            add_one = 1;
        }
    }

    return q0 + add_one;
}
