/*
 * A001641  A Fielder sequence: a(n) = a(n-1) + a(n-2) + a(n-4).
 * https://oeis.org/A001641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4), with a(1)=1, a(2)=3, a(3)=4,
 *            a(4)=11
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 78, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)11;
    
    seq_t a1 = (seq_t)1;
    seq_t a2 = (seq_t)3;
    seq_t a3 = (seq_t)4;
    seq_t a4 = (seq_t)11;
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t an = a4 + a3 + a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    
    return a4;
}
