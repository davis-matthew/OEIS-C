/*
 * A004256  a(n) = n^2*(n+1)*(n+2)^2/6.
 * https://oeis.org/A004256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n+1)*(n+2)^2/6 = n*(n+2)*binomial(n+2,3)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t x = n, y = n + 1, z = n + 2;
    if (x % 2 == 0) x /= 2; else y /= 2;
    if (n % 3 == 0) x /= 3;
    else if (n % 3 == 2) y /= 3;
    else z /= 3;
    seq_t c = (seq_t)x * (seq_t)y * (seq_t)z;
    return (seq_t)n * (seq_t)(n + 2) * c;
}
