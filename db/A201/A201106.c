/*
 * A201106  a(n) = binomial(n^2,3)/(2*n).
 * https://oeis.org/A201106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(n^3+n^2-2*n-2)/12
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arbitrary‑precision arithmetic */
    seq_t nn = (seq_t)n;

    /* Basic powers of n in seq_t */
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    /* Intermediate constants as seq_t */
    const seq_t two = (seq_t)2;
    const seq_t twelfth = (seq_t)12;

    /* (n^3 + n^2 - 2*n - 2) */
    seq_t t = n3 + n2 - (two * nn) - two;

    /* (n-1)*n*(t) / 12 */
    seq_t result = (nn - (seq_t)1) * nn * t / twelfth;

    return result;
}
