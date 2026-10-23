/*
 * A026474  a(n) = least positive integer > a(n-1) and not equal to a(i)+a(j)
 *          or a(i)+a(j)+a(k) for 1<=i<j<k<n (a 3-Stohr sequence).
 * https://oeis.org/A026474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, 2, 4, 7*(n-3)+1 (for n > 3)
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the least positive integer > a(n-1) that is not a sum of
   any two or three earlier terms.  The first three terms are
   fixed; for n > 3 the sequence is linear. */

seq_t a(idx_t n)
{
    /* handle the first three terms explicitly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    /* for n > 3 use the closed‑form 7*(n-3)+1 */
    return (seq_t)7 * ((seq_t)n - 3) + (seq_t)1;
}
