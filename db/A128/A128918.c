/*
 * A128918  a(n) = n*(n+1)/2 if n is odd, otherwise (n-1)*n/2 + 1.
 * https://oeis.org/A128918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 if n is odd, * a(n) = (n-1)*n/2 + 1 if n is
 *            even. * This implements OEIS A128918
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Early return for the special case a(0) */
    if (n == 0)
        return (seq_t)1;

    /* Odd indices: a(n) = n*(n+1)/2 */
    if ((n & 1) != 0) {
        seq_t nn = (seq_t)n;
        return (nn * (nn + 1)) / 2;
    }

    /* Even indices greater than 0: a(n) = ((n/2)*(n-1)) + 1 */
    seq_t nn = (seq_t)n;
    return ((nn / 2) * (nn - 1)) + 1;
}
