/*
 * A002537  a(2n) = a(2n-1) + 3a(2n-2), a(2n+1) = 2a(2n) + 3a(2n-1).
 * https://oeis.org/A002537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-2) - 9*a(n-4), with a(0)=1, a(1)=1, a(2)=4, a(3)=11
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)11;
    
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)4;
    seq_t a3 = (seq_t)11;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t next = (seq_t)8 * a2 - (seq_t)9 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    
    return a3;
}
