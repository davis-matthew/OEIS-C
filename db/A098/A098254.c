/*
 * A098254  Chebyshev polynomials S(n,443).
 * https://oeis.org/A098254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 443*a(n-1) - a(n-2), a(0) = 1, a(1) = 443
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base cases directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)443;

    /* Iterate the simple linear recurrence.  All arithmetic is performed
     * in type seq_t to preserve precision for values that fit in seq_t.
     */
    seq_t a_prev = (seq_t)1;   /* a(0) */
    seq_t a_curr = (seq_t)443; /* a(1) */
    seq_t a_next;

    for (idx_t i = 2; i <= n; ++i) {
        a_next = (seq_t)443 * a_curr - a_prev;
        a_prev = a_curr;
        a_curr = a_next;
    }

    return a_curr;
}
