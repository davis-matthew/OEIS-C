/*
 * A026394  a(n) = T(n,n-3), where T is the array in A026386.
 * https://oeis.org/A026394
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n^3 - 60*n^2 + 148*n - 128)/16 for n even a(n) =
 *            (9*n^3 - 51*n^2 + 95*n - 53)/16 for n odd (valid for n ≥ 3;
 *            values for n < 3 are defined as 0)
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 902, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;

    if (n & 1) {                 /* odd n */
        seq_t num = 9 * n3;
        num -= 51 * n2;
        num += 95 * ns;
        num -= 53;
        return num / 16;
    } else {                      /* even n */
        seq_t num = 9 * n3;
        num -= 60 * n2;
        num += 148 * ns;
        num -= 128;
        return num / 16;
    }
}
