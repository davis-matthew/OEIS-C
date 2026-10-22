/*
 * A002538  Second-order Eulerian numbers <<n+1,n-1>>.
 * https://oeis.org/A002538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (n+2)*a(n-1) + n*n!, n>=1, a(0):=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)0;
    seq_t factorial = (seq_t)1;
    
    for (idx_t i = 1; i <= n; i++) {
        seq_t si = (seq_t)i;
        factorial *= si;
        result = (si + (seq_t)2) * result + si * factorial;
    }
    
    return result;
}
