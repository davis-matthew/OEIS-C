/*
 * A032091  Number of reversible strings with n-1 beads of 2 colors. 4 beads
 *          are black. String is not palindromic.
 * https://oeis.org/A032091
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 - 10*n^3 + 32*n^2 - 32*n)/48 if n is even, (n^4 -
 *            10*n^3 + 32*n^2 - 38*n + 15)/48 if n is odd. Offset 6
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 373, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is defined for n >= 6; the function returns 0 for n < 6. */

seq_t a(idx_t n)
{
    if (n < 6)
        return 0;          /* value undefined below the offset */

    seq_t N   = (seq_t)n;
    seq_t n2  = N * N;     /* n^2  */
    seq_t n3  = n2 * N;    /* n^3  */
    seq_t n4  = n3 * N;    /* n^4  */

    seq_t term;
    if ((n & 1) == 0) {    /* n even */
        term = n4
             - ((seq_t)10 * n3)
             + ((seq_t)32 * n2)
             - ((seq_t)32 * N);
    } else {               /* n odd */
        term = n4
             - ((seq_t)10 * n3)
             + ((seq_t)32 * n2)
             - ((seq_t)38 * N)
             + (seq_t)15;
    }

    return term / (seq_t)48;
}
