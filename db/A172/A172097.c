/*
 * A172097  Table T(n,k) read by rows which contains in row n the 2^n terms of
 *          A171968 starting at the (n+1)st 3.
 * https://oeis.org/A172097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 3 + 4*(n - (2^r - 1)), where r = floor_log2(n + 1)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log2(v)) for v > 0 using only integer shifts. */
static idx_t floor_log2_idx(idx_t v)
{
    idx_t r = 0;
    while (v > 1) {
        v >>= 1;
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* r is the row index */
    idx_t r = floor_log2_idx(n + 1);
    /* first index of this row is 2^r - 1 */
    idx_t start = (idx_t)((uint64_t)1 << r) - 1;
    /* offset of n within the row */
    seq_t offset = (seq_t)n - (seq_t)start;
    /* value in the row increments by 4 each step */
    return ((seq_t)3) + ((seq_t)4) * offset;
}
