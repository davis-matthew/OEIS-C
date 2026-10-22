/*
 * A001635  A Fielder sequence: a(n) = a(n-1) + a(n-2) - a(n-6), n >= 7.
 * https://oeis.org/A001635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-6), n >= 7
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 51, u64 n <= 103,
 *            u128 n <= 207, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;
    if (n == 4) return (seq_t)6;
    if (n == 5) return (seq_t)10;
    if (n == 6) return (seq_t)11;
    
    seq_t a1 = (seq_t)0, a2 = (seq_t)2, a3 = (seq_t)3,
          a4 = (seq_t)6, a5 = (seq_t)10, a6 = (seq_t)11;
    
    for (idx_t i = 7; i <= n; i++) {
        seq_t next = a6 + a5 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = a6;
        a6 = next;
    }
    
    return a6;
}
