/*
 * A048510  a(n) = T(5,n), array T given by A048505.
 * https://oeis.org/A048510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+21*n+100)*2^(n-2) - 24
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 23, u64 n <= 53, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two negative‑exponent cases explicitly */
    if (n == 0) {
        seq_t val = ((seq_t)100);
        val >>= 2;               /* divide by 4 */
        return val - 24;
    }
    if (n == 1) {
        seq_t val = ((seq_t)122);
        val >>= 1;               /* divide by 2 */
        return val - 24;
    }

    /* General case for n >= 2 */
    seq_t coeff = ((seq_t)n * (seq_t)n) + (21 * (seq_t)n) + 100;
    seq_t pow2  = ((seq_t)1) << (n - 2);   /* 2^(n-2) */
    seq_t result = coeff * pow2;
    result -= 24;
    return result;
}
