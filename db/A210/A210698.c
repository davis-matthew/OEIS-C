/*
 * A210698  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant = 0 (mod 3).
 * https://oeis.org/A210698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A210698 a(n) = { 11*n^4/27 if n mod 3 == 0, (11*n^4 - 8*n^3 +
 *            6*n^2 + 4*n + 14)/27 if n mod 3 == 1, (11*n^4 - 16*n^3 + 24*n^2
 *            + 32*n + 8)/27 if n mod 3 == 2 } for n >= 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 145, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;        /* n^2 */
    seq_t n3 = n2 * ns;        /* n^3 */
    seq_t n4 = n2 * n2;        /* n^4 */

    idx_t mod = n % 3;
    seq_t num;

    switch (mod) {
        case 0:                 /* n ≡ 0 (mod 3) */
            num = 11 * n4;
            break;
        case 1:                 /* n ≡ 1 (mod 3) */
            num = 11 * n4 - 8 * n3 + 6 * n2 + 4 * ns + 14;
            break;
        default:                /* n ≡ 2 (mod 3) */
            num = 11 * n4 - 16 * n3 + 24 * n2 + 32 * ns + 8;
            break;
    }

    return num / 27;
}
