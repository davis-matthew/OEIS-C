/*
 * A003664  a(n) is smallest number larger than a(n-1) and not = a(j) + a(k),
 *          j < k.
 * https://oeis.org/A003664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th number equal to 10 or congruent to 2, 5, 6, 9,
 *            or 13 modulo 15. First 6 terms {2, 5, 6, 9, 10, 13}; subsequent
 *            groups of 5 repeat pattern {2, 5, 6, 9, 13} + 15k
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    static const seq_t first_six[] = {2, 5, 6, 9, 10, 13};
    static const seq_t offsets[] = {2, 5, 6, 9, 13};
    
    if (n <= 6) {
        return first_six[n - 1];
    }
    
    idx_t k = n - 7;
    idx_t q = k / 5;
    idx_t r = k % 5;
    
    return offsets[r] + (seq_t)(15 * (q + 1));
}
