/*
 * A003274  Number of key permutations of length n: permutations {a_i} with
 *          |a_i - a_{i-1}| = 1 or 2.
 * https://oeis.org/A003274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 2*a(n-4) + a(n-5) for n
 *            >= 7
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 53, u64 n <= 111,
 *            u128 n <= 227, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)12;
    if (n == 5) return (seq_t)20;
    if (n == 6) return (seq_t)34;
    
    seq_t buf[5] = {(seq_t)2, (seq_t)6, (seq_t)12, (seq_t)20, (seq_t)34};
    
    for (idx_t i = 7; i <= n; i++) {
        seq_t pos = (seq_t)3 * buf[4] + (seq_t)2 * buf[2] + buf[0];
        seq_t neg = (seq_t)3 * buf[3] + (seq_t)2 * buf[1];
        seq_t next = pos - neg;
        
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = next;
    }
    
    return buf[4];
}
