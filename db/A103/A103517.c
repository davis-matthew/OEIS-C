/*
 * A103517  Expansion of g.f. (1+2*x-x^2)/(1-x)^2.
 * https://oeis.org/A103517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n + 2 - 0^n
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* 0^n is 1 for n==0 and 0 otherwise; use this directly. */
    seq_t result = (seq_t)n * 2 + 2;
    if (n == 0) result -= 1;  /* subtract 1 to account for 0^0 == 1 */
    return result;
}
