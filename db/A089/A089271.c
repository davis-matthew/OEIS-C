/*
 * A089271  Third column (k=4) of array A078739(n,k) ((2,2)-generalized
 *          Stirling2).
 * https://oeis.org/A089271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*12^n - 6*6^n + 2^n (a(0) = 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A089271, the third column (k=4) of
 * the (2,2)-generalized Stirling2 array A078739.  The function
 * works for any seq_t type provided by the build (uint8_t … uint64_t,
 * unsigned __int128, or arbitrary‑precision integer class).
 * It runs in O(n) time and O(1) space, suitable for n up to at least
 * 10 000.  No external libraries are used. */
seq_t a(idx_t n)
{
    /* Handle the special offset value */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute the powers 12^n, 6^n and 2^n iteratively */
    seq_t pow12 = (seq_t)1;
    seq_t pow6  = (seq_t)1;
    seq_t pow2  = (seq_t)1;

    for (idx_t i = 0; i < n; ++i) {
        pow12 *= (seq_t)12;
        pow6  *= (seq_t)6;
        pow2  *= (seq_t)2;
    }

    /* a(n) = 6*12^n - 6*6^n + 2^n */
    seq_t term12 = pow12 * (seq_t)6;
    seq_t term6  = pow6  * (seq_t)6;

    return term12 - term6 + pow2;
}
