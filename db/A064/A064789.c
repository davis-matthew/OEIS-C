/*
 * A064789  Inverse permutation to A057028.
 * https://oeis.org/A064789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k*(k+1)/2 + |m| + [m <= 0] where k = floor((sqrt(8*n) -
 *            1)/2) m = 2*(n-1) - k*(k+2)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute k such that k*(k+1)/2 < n <= (k+1)*(k+2)/2
       (this gives k = floor((sqrt(8*n)-1)/2) ) */
    uint64_t k = 0;
    while ( ((k + 1) * (k + 2)) / 2 < n )
        ++k;

    /* m = 2*(n-1) - k*(k+2)  (use signed arithmetic) */
    int64_t m = (int64_t)2 * ((int64_t)n - 1) - (int64_t)k * (k + 2);

    /* absolute value of m */
    seq_t abs_m = (m >= 0) ? (seq_t)m : (seq_t)(-m);

    /* triangular base: k*(k+1)/2  (k is zero‑based row index) */
    seq_t base = (seq_t)k * (k + 1) / 2;

    /* indicator [m <= 0] */
    seq_t add_flag = (m <= 0) ? 1 : 0;

    return base + abs_m + add_flag;
}
