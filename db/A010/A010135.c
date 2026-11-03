/*
 * A010135  Continued fraction for sqrt(45).
 * https://oeis.org/A010135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 91 terms through n = 9265 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A010135: continued fraction quotients of sqrt(45)
   a(0)=6; for n>0, the period is 6: [1,2,2,2,1,12], so
   a(n) = PERIOD[(n-1)%6]  */
seq_t a(idx_t n)
{
    static const seq_t PERIOD[6] = {1, 2, 2, 2, 1, 12};
    if (n == 0) return 6;
    return PERIOD[(n - 1) % 6];
}
