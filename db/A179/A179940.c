/*
 * A179940  Number of times n appears in a 10 X 10 multiplication table.
 * https://oeis.org/A179940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (i, j) with 1 <= i, j <= 10 such
 *            that i * j = n. For n > 100 the result is 0
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n > 100) return 0;

    seq_t count = 0;
    seq_t sn = n;

    for (idx_t d = 1; d <= 10; ++d) {
        if (sn % d == 0) {
            seq_t e = sn / d;
            if (e <= 10) ++count;
        }
    }
    return count;
}
