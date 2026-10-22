/*
 * A004041  Scaled sums of odd reciprocals: a(n) = (2*n + 1)!!*(Sum_{k=0..n}
 *          1/(2*k + 1)).
 * https://oeis.org/A004041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n*a(n-1) - (2*n - 1)^2*a(n-2), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 1, cur = 4;
    idx_t i;
    if (n == 0) return prev;
    for (i = 2; i <= n; i++) {
        seq_t c = 2 * (seq_t)i - 1;
        seq_t next = 4 * (seq_t)i * cur - c * c * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
