/*
 * A082811  Smallest multiple of n beginning with 2.
 * https://oeis.org/A082811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest multiple of n whose decimal representation
 *            starts with 2. Algorithm: test successive multiples k*n,
 *            k=1,2,3,... and return the first for which the most significant
 *            digit equals 2. The search stops quickly (never exceeds 14
 *            iterations in practice)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* iterate over integer multiples until a leading 2 is found */
    for (idx_t k = 1;; ++k) {
        seq_t prod = (seq_t)n * (seq_t)k;   /* compute k*n */
        seq_t tmp  = prod;
        /* strip trailing digits until only the most significant digit remains */
        while (tmp >= (seq_t)10) {
            tmp /= (seq_t)10;
        }
        if (tmp == (seq_t)2) {
            return prod;   /* found the smallest multiple starting with 2 */
        }
    }
}
