/*
 * A004247  Multiplication table read by antidiagonals: T(i,j) = i*j (i>=0,
 *          j>=0). Alternatively, multiplication triangle read by rows: P(i,j)
 *          = j*(i-j) (i>=0, 0<=j<=i).
 * https://oeis.org/A004247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   triangle by rows: a(n) = P(i,j) = j*(i-j), where n = i*(i+1)/2 +
 *            j, 0 <= j <= i
 * Range:     u8 n <= 543, u16 n <= 1325, u32 n <= 1325, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t i = 0;
    while ((i + 1) * (i + 2) / 2 <= n) {
        i++;
    }
    idx_t j = n - i * (i + 1) / 2;
    return (seq_t)j * (seq_t)(i - j);
}
