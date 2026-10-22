/*
 * A001639  A Fielder sequence. a(n) = a(n-1) + a(n-3) + a(n-4) + a(n-5), n >=
 *          6.
 * https://oeis.org/A001639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) + a(n-5), n >= 6, with a(1)=1,
 *            a(2)=1, a(3)=4, a(4)=9, a(5)=16
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 42, u64 n <= 85, u128 n <= 170,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 5) {
        const seq_t base[6] = {0, 1, 1, 4, 9, 16};
        return base[n];
    }
    
    seq_t a1 = 1, a2 = 1, a3 = 4, a4 = 9, a5 = 16;
    
    for (idx_t i = 6; i <= n; i++) {
        seq_t next = a5 + a3 + a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    
    return a5;
}
