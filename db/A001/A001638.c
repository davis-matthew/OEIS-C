/*
 * A001638  A Fielder sequence: a(n) = a(n-1) + a(n-3) + a(n-4), n >= 4.
 * https://oeis.org/A001638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4), with a(0)=4, a(1)=1, a(2)=1,
 *            a(3)=4
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;
    
    seq_t a0 = (seq_t)4;
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)1;
    seq_t a3 = (seq_t)4;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t next = a3 + a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    
    return a3;
}
