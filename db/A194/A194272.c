/*
 * A194272  Array T(n,k) with 6 columns read by rows in which row n lists
 *          3*n-2, 3*n-1, 3*n, 3*n, 3*n, 3*n.
 * https://oeis.org/A194272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = floor((n+3)/6) + floor((n+4)/6) + floor((n+5)/6)
 * Range:     u8 n <= 322, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A194272.  The sequence can be computed from
   this closed form for any n ≥ 1; no memoisation or recursion is
   required. */
seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;          /* convert index to seq_t to avoid overflow in intermediates */
    return (k + 3) / 6 +        /* floor((n+3)/6)   */
           (k + 4) / 6 +        /* floor((n+4)/6)   */
           (k + 5) / 6;        /* floor((n+5)/6)   */
}
