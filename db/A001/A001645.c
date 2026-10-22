/*
 * A001645  A Fielder sequence.
 * https://oeis.org/A001645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-5) for n >= 6, with
 *            initial terms a(1)=1, a(2)=3, a(3)=7, a(4)=11, a(5)=26
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 69, u128 n <= 139,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 3;
    if (n == 3) return 7;
    if (n == 4) return 11;
    if (n == 5) return 26;
    
    seq_t a1 = 1, a2 = 3, a3 = 7, a4 = 11, a5 = 26;
    
    for (idx_t i = 6; i <= n; i++) {
        seq_t next = a5 + a4 + a3 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = next;
    }
    
    return a5;
}
