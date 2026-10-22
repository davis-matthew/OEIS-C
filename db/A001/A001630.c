/*
 * A001630  Tetranacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4), with
 *          a(0)=a(1)=0, a(2)=1, a(3)=2.
 * https://oeis.org/A001630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4), a(0)=0, a(1)=0,
 *            a(2)=1, a(3)=2
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 2;
    
    seq_t v0 = 0, v1 = 0, v2 = 1, v3 = 2;
    
    for (idx_t i = 4; i <= n; i++) {
        seq_t vnew = v0 + v1 + v2 + v3;
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = vnew;
    }
    
    return v3;
}
