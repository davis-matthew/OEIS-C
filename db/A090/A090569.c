/*
 * A090569  The survivor w(n,2) in a modified Josephus problem, with a step of
 *          2.
 * https://oeis.org/A090569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 1 + Σ_{odd j} b_j * 2^j, where b_j are the bits of
 * m = n  if n is odd, otherwise m = n-1 (hence m is odd).
 * This implements OEIS A090569, the survivor w(n,2) in the modified
 * Josephus problem with step 2.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* safety for unexpected zero input */

    /* m is the odd number among n and n-1 */
    idx_t m = (n & 1) ? n : n - 1;

    /* mask with 1s on all odd bit positions (bit 0 is LSB) */
    const uint64_t MASK_ODD = 0xAAAAAAAAAAAAAAAAULL;

    /* the weighted sum of odd-positioned bits */
    seq_t odds = (seq_t)(m & MASK_ODD);

    return odds + 1;
}
