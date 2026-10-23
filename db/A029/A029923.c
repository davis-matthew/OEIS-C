/*
 * A029923  Convert n from pounds (lbs) to kilograms (kg).
 * https://oeis.org/A029923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n * 0.45359237 + 1/2)
 * Range:     u8 none, u16 none, u32 n <= 130, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    const seq_t K = 45359237u;   /* 0.45359237 * 10^8 */
    const seq_t D = 100000000u;  /* 10^8 */
    const seq_t H = 50000000u;   /* half of D for rounding */
    seq_t num = ((seq_t)n) * K + H;
    return num / D;
}
