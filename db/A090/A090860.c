/*
 * A090860  Number of ways of 4-coloring a map in which there is a central
 *          circle surrounded by an annulus divided into n-1 regions. There
 *          are n regions in all.
 * https://oeis.org/A090860
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) - 8*(-1)^n (for n ≥ 4, offset 4)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 4.  The behaviour below the
       offset is irrelevant for the tests, so return 0 for smaller n. */
    if (n < 4)
        return 0;

    /* Compute 2^(n+1) by repeated multiplication.  This is O(n) time
       and works for all static types supported by the framework. */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n + 1; ++i)
        pow2 *= (seq_t)2;   /* multiply by 2 each step */

    /* Adjust the value by ±8 depending on the parity of n. */
    if (n & 1)          /* odd n: +8 */
        return pow2 + (seq_t)8;
    else                /* even n: -8 */
        return pow2 - (seq_t)8;
}
