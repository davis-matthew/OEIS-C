/*
 * A007807  A variation on Euclid: a(n)=g(n)-1, where g(0)=0, g(1)=1,
 *          g(n+1)=g(n)(g(n-1)+1).
 * https://oeis.org/A007807
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = g(n) - 1, where g(0) = 0, g(1) = 1, g(n+1) = g(n) *
 *            (g(n-1) + 1)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t g_prev = 1;
    seq_t g_curr = 1;
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t g_next = g_curr * (g_prev + 1);
        g_prev = g_curr;
        g_curr = g_next;
    }
    
    return g_curr - 1;
}
