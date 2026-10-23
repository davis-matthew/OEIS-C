/*
 * A175629  Legendre symbol (n,7).
 * https://oeis.org/A175629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, mult, sign
 * Formula:   a(n) = 0 if n ≡ 0 (mod 7), 1 if n ≡ 1,2,4 (mod 7), -1 otherwise
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n % 7;
    seq_t result;
    if (m == 0) {
        result = 0;
    } else if (m == 1 || m == 2 || m == 4) {
        result = 1;
    } else {
        result = -1;
    }
    return result;
}
