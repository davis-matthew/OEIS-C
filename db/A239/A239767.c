/*
 * A239767  Degrees of polynomial on the fermionic side of the finite
 *          generalization of identity 46 from Slater's List.
 * https://oeis.org/A239767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*(10*n^2 + 2*(1+(-1)^n)*n - (1-(-1)^n))
 * Range:     u8 n <= 14, u16 n <= 228, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent piecewise implementation using parity of n. */
seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid intermediate overflow */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    if ((n & 1ULL) == 0) {       /* n even */
        /* numerator: 10*n^2 + 4*n */
        return (10 * n2 + 4 * nn) / 8;
    } else {                     /* n odd */
        /* numerator: 10*n^2 - 2 */
        return (10 * n2 - 2) / 8;
    }
}
