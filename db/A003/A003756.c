/*
 * A003756  Number of spanning trees with degrees 1 and 3 in S_4 X P_{2n-1}.
 * https://oeis.org/A003756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 16*a(n-2) + 4*a(n-3), n > 4 a(1) = 1, a(2) =
 *            0, a(3) = 24, a(4) = 54
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 0;
    if (n == 3) return 24;
    if (n == 4) return 54;
    
    seq_t prev3 = 0;   // a(2)
    seq_t prev2 = 24;  // a(3)
    seq_t prev1 = 54;  // a(4)
    seq_t curr;
    
    for (idx_t i = 5; i <= n; i++) {
        curr = 2 * prev1 + 16 * prev2 + 4 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
