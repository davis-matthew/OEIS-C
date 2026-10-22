/*
 * A001608  Perrin sequence (or Perrin numbers, or Ondrej Such sequence): a(n)
 *          = a(n-2) + a(n-3) with a(0) = 3, a(1) = 0, a(2) = 2.
 * https://oeis.org/A001608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-2) + a(n-3), a(0) = 3, a(1) = 0, a(2) = 2
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 78, u64 n <= 157,
 *            u128 n <= 315, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    
    seq_t a0 = (seq_t)3;
    seq_t a1 = (seq_t)0;
    seq_t a2 = (seq_t)2;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t next = a0 + a1;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    
    return a2;
}
