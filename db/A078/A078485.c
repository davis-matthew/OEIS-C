/*
 * A078485  Number of irreducible indecomposable permutations of degree n.
 * https://oeis.org/A078485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) - 1 for n > 3 Special values: a(0)=0, a(1)=1,
 *            a(2)=1, a(3)=1 The implementation uses a simple iterative
 *            doubling for the power of two. This works for
 *            arbitrary-precision seq_t and for 64‑bit builds up to the point
 *            where the result still fits
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the offset values */
    if (n == 0)
        return 0;
    if (n <= 3)
        return 1;   /* a(1)=a(2)=a(3)=1 */

    /* for n>3 we need 2^(n-1)-1 */
#if defined(_WIN64) || defined(__SIZEOF_INT128__) || defined(__int128_t)
    /* If seq_t is wide enough to contain 2^(n-1) (e.g. 128‑bit or big‑int) */
    seq_t pow2 = 1;
#else
    /* seq_t is 8/16/32/64‑bit, avoid undefined right shift */
    seq_t pow2 = 1;
#endif

    /* compute 2^(n-1) by repeated doubling */
    for (idx_t i = 1; i < n; ++i)
        pow2 += pow2;          /* pow2 *= 2 */

    return pow2 - 1;
}
