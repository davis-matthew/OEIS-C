/*
 * A184032  1/16 the number of (n+1) X 3 0..3 arrays with all 2 X 2 subblocks
 *          having the same four values.
 * https://oeis.org/A184032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 9*2^(n/2-1) + 9*2^(n-1) + 13 for n even * a(n) =
 *            9*2^(n-1) + 3*2^((n+1)/2) + 13 for n odd * (OEIS A184032, 1 ≤ n)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* power of two: 2^e, computed by repeated left‑shift.
 * Exp is an idx_t; the loop shifts one bit at a time, never
 * using a shift count ≥ the width of seq_t, so the behaviour
 * is defined for all seq_t types. */
static seq_t pow2_u64(idx_t e)
{
    seq_t r = 1;
    while (e) {
        r <<= 1;
        e--;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* behaviour below offset is irrelevant; return 0 for n==0 */
    if (n == 0)
        return (seq_t)0;

    if (n & 1ULL) {                     /* n odd */
        seq_t term1 = 9 * pow2_u64(n - 1);
        seq_t term2 = 3 * pow2_u64((n + 1) / 2);
        return term1 + term2 + (seq_t)13;
    } else {                           /* n even */
        seq_t term1 = 9 * pow2_u64(n / 2 - 1);
        seq_t term2 = 9 * pow2_u64(n - 1);
        return term1 + term2 + (seq_t)13;
    }
}
