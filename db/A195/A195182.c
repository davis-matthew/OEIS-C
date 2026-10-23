/*
 * A195182  a(n) = 6*n - floor(2*n*e).
 * https://oeis.org/A195182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*n - floor(2*n*e)
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 2*e ≈ 5.436563656918090470720...
       Multiply by 1e12 to preserve 12 decimal digits.  Since n < 10000
       and 10000 << 1e12, the truncation error is less than 1/(n*Q);
       thus floor(n*P/Q) equals floor(n*2*e). */
    const seq_t P = 5436563656918ULL;   /* floor(2*e * 1e12) */
    const seq_t Q = 1000000000000ULL;   /* 1e12 */

    unsigned __int128 prod = (unsigned __int128)P * (unsigned __int128)n;
    seq_t floor_val = (seq_t)(prod / Q);

    seq_t six_n = (seq_t)n * 6;

    return six_n - floor_val;
}
