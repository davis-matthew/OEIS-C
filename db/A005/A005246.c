/*
 * A005246  a(n) = (1 + a(n-1)*a(n-2))/a(n-3), a(0) = a(1) = a(2) = 1.
 * https://oeis.org/A005246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 4*a(n-2) - a(n-4), a(0) = a(1) = a(2) = 1, a(3) = 2
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 69, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 2) return 1;
    if (n == 3) return 2;
    
    seq_t prev_prev, prev, curr;
    
    if (n % 2 == 0) {
        prev_prev = 1;
        prev = 1;
        for (idx_t i = 4; i <= n; i += 2) {
            curr = 4 * prev - prev_prev;
            prev_prev = prev;
            prev = curr;
        }
        return prev;
    } else {
        prev_prev = 1;
        prev = 2;
        for (idx_t i = 5; i <= n; i += 2) {
            curr = 4 * prev - prev_prev;
            prev_prev = prev;
            prev = curr;
        }
        return prev;
    }
}
