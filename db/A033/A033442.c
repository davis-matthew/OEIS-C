/*
 * A033442  Number of edges in 10-partite Turán graph of order n.
 * https://oeis.org/A033442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} floor(9*i/10)
 * Range:     u8 n <= 23, u16 n <= 381, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t q = n / 10;
    idx_t r = n % 10;
    seq_t result = (seq_t)45 * (seq_t)q * (seq_t)q
                   + (seq_t)9 * (seq_t)q * (seq_t)r;
    for (idx_t j = 1; j <= r; ++j) {
        result += (seq_t)9 * (seq_t)j / 10;
    }
    return result;
}
