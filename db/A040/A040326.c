/*
 * A040326  Continued fraction for sqrt(345).
 * https://oeis.org/A040326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction terms of sqrt(345)  
   a(0) = ⌊√345⌋ = 18, afterwards the pattern  
   1,1,2,1,6,1,2,1,1,36 repeats forever. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)18;
    static const seq_t period[10] = {1, 1, 2, 1, 6, 1, 2, 1, 1, 36};
    /* (n-1) is safe because n>0 here */
    return period[(n - 1u) % (idx_t)10u];
}
