/*
 * A211623  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...-1,1,...,n} and -1<=w+2x+3y<=1.
 * https://oeis.org/A211623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, n = 0 Formula: a(n) = 4*n^2 - 3*n + (n%2?1:2), n > 0
 *            (From OEIS A211623)
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    /* Compute using 64‑bit intermediates to avoid overflow on
       small seq_t types while staying within the 64‑bit limit
       for the given range (n up to about 10 000). */
    idx_t fn   = n;
    idx_t res64 = 4 * fn * fn - 3 * fn + ((fn & 1) ? 1 : 2);
    return (seq_t)res64;
}
