/*
 * A000449  Rencontres numbers: number of permutations of [n] with exactly 3
 *          fixed points.
 * https://oeis.org/A000449
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*a(n-1) - (-1)^n*n*(n-1)*(n-2)/6, a(3) = 1, a(0) = a(1)
 *            = a(2) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (n == 3) return 1;
    if (n == 4) return 0;
    
    seq_t a_prev = 0;  /* a(4) */
    
    for (idx_t i = 5; i <= n; i++) {
        seq_t correction = (seq_t)i * ((seq_t)i - 1) * ((seq_t)i - 2) / 6;
        seq_t a_curr;
        
        if (i % 2 == 1) {
            /* i odd: (-1)^i = -1 */
            a_curr = (seq_t)i * a_prev + correction;
        } else {
            /* i even: (-1)^i = 1 */
            a_curr = (seq_t)i * a_prev - correction;
        }
        
        a_prev = a_curr;
    }
    
    return a_prev;
}
