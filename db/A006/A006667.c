/*
 * A006667  Number of tripling steps to reach 1 from n in '3x+1' problem, or
 *          -1 if 1 is never reached.
 * https://oeis.org/A006667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, hear, changed
 * Formula:   a(n) = a(n/2) if n is even; a(n) = a(3n+1)+1 if n is odd; a(1) =
 *            0
 * Range:     u8 n <= 26, u16 n <= 7662, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return 0;
    
    seq_t count = 0;
    seq_t current = (seq_t)n;
    
    while (current != 1) {
        if (current % 2 == 0) {
            current = current / 2;
        } else {
            count++;
            current = current * 3 + 1;
        }
    }
    
    return count;
}
