/*
 * A005665  Minimal number of moves for the cyclic variant of the Towers of
 *          Hanoi for 3 pegs and n disks, with the final peg one step away.
 * https://oeis.org/A005665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 3, a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 5;
    
    seq_t prev2 = 1;  // a(1)
    seq_t prev1 = 5;  // a(2)
    
    for (idx_t i = 3; i <= n; i++) {
        seq_t curr = 2 * prev1 + 2 * prev2 + 3;
        prev2 = prev1;
        prev1 = curr;
    }
    
    return prev1;
}
