/*
 * A006460  Image of n after 3k iterates of '3x+1' map (k large).
 * https://oeis.org/A006460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = value at step 3k of the Collatz trajectory, k -> infinity
 *            The trajectory enters the 1-2-4 cycle, and we find the limiting
 *            position
 * Range:     u8 n <= 38, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t values[3] = {1, 4, 2};
    
    seq_t x = (seq_t)n;
    int step_mod3 = 0;
    
    while (x != 1 && x != 2 && x != 4) {
        if (x % 2 == 0) {
            x = x / 2;
        } else {
            x = 3 * x + 1;
        }
        step_mod3 = (step_mod3 + 1) % 3;
    }
    
    int offset_x = (x == 1) ? 0 : (x == 4) ? 1 : 2;
    int final_offset = (offset_x - step_mod3 + 3) % 3;
    
    return values[final_offset];
}
