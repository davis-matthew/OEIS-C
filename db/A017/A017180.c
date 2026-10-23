/*
 * A017180  a(n) = (9n+1)^8.
 * https://oeis.org/A017180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for the OEIS sequence A017180.
   The calculation uses only integer arithmetic and
   works with any seq_t that supports the usual operators. */
seq_t a(idx_t n)
{
    /* Compute 9*n + 1 in seq_t to avoid overflow of idx_t multiplication. */
    seq_t base = (seq_t)n;
    base *= (seq_t)9;
    base += (seq_t)1;

    /* Raise base to the 8th power via repeated squaring. */
    seq_t r = base;   /* r = base^1 */
    r *= r;           /* r = base^2 */
    r *= r;           /* r = base^4 */
    r *= r;           /* r = base^8 */

    return r;
}
