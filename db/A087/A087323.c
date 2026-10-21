/*
 * A087323  a(n) = (n+1) * 2^n - 1.
 * https://oeis.org/A087323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * 2^n - 1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow2 = 1;                       /* 2^0 */
    /* Compute 2^n by repeated doubling (avoids bit‑shift overflow) */
    for (idx_t i = 0; i < n; ++i) {
        pow2 += pow2;                     /* pow2 = pow2 * 2 */
    }

    /* Multiply by (n+1) and subtract 1 */
    seq_t result = pow2 * ((seq_t)n + 1);
    result -= 1;
    return result;
}
