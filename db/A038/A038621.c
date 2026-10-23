/*
 * A038621  Growth function of an infinite cubic graph (number of nodes at
 *          distance <=n from fixed node).
 * https://oeis.org/A038621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 4; for n >= 2: if n mod 3 == 0, a(n) = (4*n^3 +
 *            6*n^2 + 15*n - 9)/9; if n mod 3 == 1, a(n) = (4*n^3 + 6*n^2 +
 *            18*n - 10)/9; if n mod 3 == 2, a(n) = (4*n^3 + 6*n^2 + 15*n +
 *            4)/9;
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;

    int r = (int)(n % 3);
    seq_t num;

    if (r == 0) {
        num = 4 * n3 + 6 * n2 + 15 * nn - (seq_t)9;
    } else if (r == 1) {
        num = 4 * n3 + 6 * n2 + 18 * nn - (seq_t)10;
    } else /* r == 2 */ {
        num = 4 * n3 + 6 * n2 + 15 * nn + (seq_t)4;
    }

    return num / (seq_t)9;
}
