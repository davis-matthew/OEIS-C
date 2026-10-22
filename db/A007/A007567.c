/*
 * A007567  Knopfmacher expansion of 1/2: a(n+1) = a(n-1)(a(n)+1)-1.
 * https://oeis.org/A007567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = a(n-2)*(a(n-1)+1) - 1 for n >= 4, a(0) = -3, a(1) = -2,
 *            a(2) = 2, a(3) = 5
 * Range:     i8 n <= 5, i16 n <= 6, i32 n <= 8, i64 n <= 9, i128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t prev, cur;
    idx_t i;
    if (n == 0) return (seq_t)(-3);
    if (n == 1) return (seq_t)(-2);
    if (n == 2) return (seq_t)2;
    prev = (seq_t)2;
    cur = (seq_t)5;
    for (i = 4; i <= n; i++) {
        seq_t next = prev * (cur + 1) - 1;
        prev = cur;
        cur = next;
    }
    return cur;
}
