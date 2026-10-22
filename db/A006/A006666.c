/*
 * A006666  Number of halving steps to reach 1 in '3x+1' problem, or -1 if
 *          this never happens.
 * https://oeis.org/A006666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look, easy
 * Formula:   modified 3x+1 map, T(n) = n/2 if even, (3n+1)/2 if odd, count
 *            steps to 1
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t current = (seq_t)n;
    seq_t steps = 0;
    
    while (current != 1) {
        if (current % 2 == 0) {
            current /= 2;
        } else {
            seq_t temp = current / 2;
            current = 3 * temp + 2;
        }
        steps++;
    }
    
    return steps;
}
