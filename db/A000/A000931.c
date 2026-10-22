/*
 * A000931  Padovan sequence (or Padovan numbers): a(n) = a(n-2) + a(n-3) with
 *          a(0) = 1, a(1) = a(2) = 0.
 * https://oeis.org/A000931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-2) + a(n-3), a(0) = 1, a(1) = 0, a(2) = 0
 * Range:     u8 n <= 25, u16 n <= 45, u32 n <= 85, u64 n <= 163,
 *            u128 n <= 321, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1 || n == 2) return 0;
    
    seq_t prev3 = 1;  // a(0)
    seq_t prev2 = 0;  // a(1)
    seq_t prev1 = 0;  // a(2)
    seq_t curr;
    
    for (idx_t i = 3; i <= n; i++) {
        curr = prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
