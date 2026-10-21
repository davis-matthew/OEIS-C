/*
 * A083072  A subdiagonal of number array A083064.
 * https://oeis.org/A083072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((n+2)*(n+4)^n + 1)/(n+3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t type */
    seq_t N = (seq_t)n;

    /* When seq_t is small (≤ 64 bits) use 128‑bit arithmetic
     * to avoid overflow for the values that fit in seq_t.
     * For larger seq_t (128 bits or arbitrary precision) use pow_seq
     * which produces the exact value in the target type.
     */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit intermediate computations */
        unsigned __int128 base = (unsigned __int128)(N + (seq_t)4);
        unsigned __int128 pow128 = 1;
        /* compute base^n */
        for (idx_t i = 0; i < n; ++i)
            pow128 *= base;

        unsigned __int128 num = (unsigned __int128)(N + (seq_t)2) * pow128 + (unsigned __int128)1;
        unsigned __int128 den = (unsigned __int128)(N + (seq_t)3);
        unsigned __int128 res = num / den;
        return (seq_t)res;
    } else {
        /* seq_t is 128 bits or arbitrary precision */
        seq_t base = N + (seq_t)4;
        seq_t pow_val = pow_seq(base, (int)n);
        seq_t num = (N + (seq_t)2) * pow_val + (seq_t)1;
        seq_t den = N + (seq_t)3;
        return num / den;
    }
}
