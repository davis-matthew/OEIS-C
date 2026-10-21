/*
 * A053717  a(n) = n^7 + n^6 + n^5 + n^4 + n^3 + n^2 + n + 1.
 * https://oeis.org/A053717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^7 + n^6 + n^5 + n^4 + n^3 + n^2 + n + 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 23, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0) = 1, a(1) = 8, a(2) = 255, ...  as per OEIS A053717 */

seq_t a(idx_t n)
{
    /* If seq_t is unsigned integer, intermediate products may overflow.
       The sequence values are checked only when they fit in seq_t, so this
       implementation is adequate for all supported builds. */
    seq_t sum = 1;          /* n^0 */
    seq_t term = 1;         /* current n^i, starts at 1 for i = 0 */

    for (idx_t i = 1; i <= 7; ++i) {
        term *= (seq_t)n;   /* term = n^i */
        sum += term;        /* accumulate */
    }
    return sum;
}
