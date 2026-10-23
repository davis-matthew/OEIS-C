/*
 * A108922  Expansion of 1/((x^8+1)*(x-1)^2).
 * https://oeis.org/A108922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0}^{⌊n/8⌋} (-1)^k * (n - 8k + 1)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 8;               /* floor(n/8) */
    if ((m & 1) == 0) {            /* m even */
        seq_t half = (seq_t)(m >> 1);   /* m/2 */
        return (seq_t)n + 1 - 8 * half;
    } else {                       /* m odd */
        return 4 * ((seq_t)m + 1);
    }
}
