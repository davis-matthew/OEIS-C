/*
 * A052552  a(2*n+1) = 1, a(2*n) = 2*a(2*n-2) - 1.
 * https://oeis.org/A052552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2*n)=2^n+1, a(2*n+1)=1
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A052552 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Odd indices: always 1 */
    if (n & 1) return 1;

    /* Even index: a(2*k) = 2^k + 1 */
    idx_t k = n >> 1;      /* k = n/2 */
    seq_t power = 1;       /* 2^0 */

    /* Multiply by 2, k times.  Looping avoids UB from shifting by width. */
    for (idx_t i = 0; i < k; ++i) {
        power *= 2;
    }

    return power + 1;
}
