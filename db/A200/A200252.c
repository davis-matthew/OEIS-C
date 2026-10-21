/*
 * A200252  Number of 0..n arrays x(0..2) of 3 elements with each no smaller
 *          than the sum of its previous elements modulo (n+1).
 * https://oeis.org/A200252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 5*n^2 + 8*n + 4)/4 for even n * a(n) = (n^3 +
 *            5*n^2 + 9*n + 5)/4 for odd n (n ≥ 1) * This matches the OEIS
 *            A200252 definition
 * Range:     u8 n <= 8, u16 n <= 62, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* ignore calls with n < 1 (undefined behaviour for the sequence) */
    if (n < 1) return 0;

    /* compute in idx_t to avoid overflow of seq_t when it is small */
    idx_t nn = n;
    idx_t n2 = nn * nn;          /* n^2   */
    idx_t n3 = n2 * nn;          /* n^3   */

    idx_t carry;
    if (nn & 1) {                /* odd n */
        carry = n3 + 5*n2 + 9*nn + 5;
    } else {                     /* even n */
        carry = n3 + 5*n2 + 8*nn + 4;
    }

    carry /= 4;
    return (seq_t)carry;
}
