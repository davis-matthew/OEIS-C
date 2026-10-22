/*
 * A007568  Knopfmacher expansion of 2/3: a(n+1) = a(n-1)(a(n)+1)-1.
 * https://oeis.org/A007568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = a(n-1)*(a(n)+1) - 1 for n >= 2, a(0) = 4, a(1) = 3,
 *            a(2) = 3
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev = 3;
    seq_t cur = 3;
    idx_t i;
    if (n == 0) return (seq_t)4;
    if (n <= 2) return (seq_t)3;
    for (i = 2; i < n; i++) {
        seq_t next = prev * (cur + 1) - 1;
        prev = cur;
        cur = next;
    }
    return cur;
}
