/*
 * A006253  Number of perfect matchings (or domino tilings) in C_4 X P_n.
 * https://oeis.org/A006253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 2*(-1)^n, a(0) = 1, a(1) = 2, a(2) =
 *            9
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 9;
    
    seq_t prev2 = 2;    // a(1)
    seq_t prev1 = 9;    // a(2)
    seq_t curr;
    
    for (idx_t i = 3; i <= n; i++) {
        if (i & 1) {
            // odd n: a(n) = 4*a(n-1) - a(n-2) - 2
            curr = 4 * prev1 - prev2 - 2;
        } else {
            // even n: a(n) = 4*a(n-1) - a(n-2) + 2
            curr = 4 * prev1 - prev2 + 2;
        }
        prev2 = prev1;
        prev1 = curr;
    }
    
    return curr;
}
