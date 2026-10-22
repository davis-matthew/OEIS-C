/*
 * A005460  a(n) = (3*n+4)*(n+3)!/24.
 * https://oeis.org/A005460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+4)*(n+3)!/24, computed as (3*n+4) * prod_{i=5..n+3}
 *            i for n >= 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result;
    idx_t i;

    if (n == 0)
        return (seq_t)1;

    result = (seq_t)n * 3 + 4;
    for (i = 5; i <= n + 3; i++)
        result *= (seq_t)i;

    return result;
}
