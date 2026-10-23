/*
 * A138888  Non-Fermat numbers.
 * https://oeis.org/A138888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   * a(n) = n+m+2 if n+m >= 4^(2^m) * a(n) = n+m+1 otherwise *
 *            where m = floor(log2(log2(n-1))) (n > 2) * a(1)=1, a(2)=2
 * Range:     u8 n <= 16, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= (idx_t)2)
        return (seq_t)n;

    /* Compute m = floor(log2(log2(n-1))) */
    idx_t nm1 = n - 1;
    unsigned l1 = floor_log2_uint64(nm1);   /* floor(log2(n-1))      */
    unsigned m  = floor_log2_uint64(l1);   /* floor(log2(l1))      */

    /* n + m is the left side of the test */
    seq_t nm = (seq_t)n + (seq_t)m;

    bool cond;
    if (m <= 5) {
        /* For m <= 5, threshold 4^(2^m) = 2^(2^(m+1)) fits in 128‑bit */
        uint64_t exp = ((uint64_t)1) << (m + 1);          /* 2^(m+1)          */
        seq_t threshold = pow2(exp);                     /* 2^(2^(m+1))       */
        cond = nm >= threshold;
    } else {
        /*
         * For m >= 6 the threshold is at least 2^128,
         * which exceeds the maximum value representable by
         * an unsigned __int128 (the largest built-in seq_t).
         * Therefore the condition n+m >= threshold can never
         * be true for any n that fits in 128 bits.  We can
         * safely set the flag to false and use the
         * "n+m+1" branch.
         */
        cond = false;
    }

    if (cond)
        return nm + 2;
    else
        return nm + 1;
}
