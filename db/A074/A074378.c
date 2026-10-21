/*
 * A074378  Even triangular numbers halved.
 * https://oeis.org/A074378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = k*(4k+1), a(2k+1) = (k+1)*(4k+3)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = half of the even triangular numbers, indexed starting at n = 0 */

seq_t a(idx_t n)
{
    /* Compute using the closed formulas.  The result is exact as long as
       the arithmetic fits into the underlying type seq_t.  For unsigned
       64‑bit seq_t this is true for all n such that the term fits into
       64 bits (which is what the test harness checks). */
    seq_t k;

    if ((n & 1ULL) == 0) {           /* n = 2k */
        k = (seq_t)(n >> 1);         /* k = n/2 */
        return k * ((seq_t)4 * k + 1);
    } else {                          /* n = 2k+1 */
        k = (seq_t)(n >> 1);         /* k = (n-1)/2 */
        return (k + 1) * ((seq_t)4 * k + 3);
    }
}
