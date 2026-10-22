/*
 * A005666  Minimal number of moves for the cyclic variant of the Towers of
 *          Hanoi for 3 pegs and n disks, with the final peg two steps away.
 * https://oeis.org/A005666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) + 3, a(0) = 0, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 2;
    
    seq_t a_prev2 = 0;
    seq_t a_prev1 = 2;
    seq_t a_curr;
    
    for (idx_t i = 2; i <= n; i++) {
        a_curr = 2 * a_prev1 + 2 * a_prev2 + 3;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    
    return a_prev1;
}
