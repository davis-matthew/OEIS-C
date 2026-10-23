/*
 * A040905  Continued fraction for sqrt(936).
 * https://oeis.org/A040905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms for sqrt(936).
   a(0) = 30, for n > 0 a(n) = period[(n-1) % 8] with
   period = {1,1,2,6,2,1,1,60}. */
seq_t a(idx_t n) {
    if (n == 0) {
        return 30;
    }
    static const seq_t period[8] = {1, 1, 2, 6, 2, 1, 1, 60};
    return period[(n - 1) & 7];   /* (n-1)%8 using bit mask for 8=2^3 */
}
