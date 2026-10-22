/*
 * A000904  a(n) = (n+1)*a(n-1) + (n+2)*a(n-2) + a(n-3); a(1)=0, a(2)=3,
 *          a(3)=13.
 * https://oeis.org/A000904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = (n+1)*a(n-1) + (n+2)*a(n-2) + a(n-3); a(1)=0, a(2)=3,
 *            a(3)=13
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    if (n == 2) return 3;
    if (n == 3) return 13;
    
    seq_t a1 = 0;   /* a(1) */
    seq_t a2 = 3;   /* a(2) */
    seq_t a3 = 13;  /* a(3) */
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t result = (seq_t)(i + 1) * a3 + (seq_t)(i + 2) * a2 + a1;
        a1 = a2;
        a2 = a3;
        a3 = result;
    }
    
    return a3;
}
