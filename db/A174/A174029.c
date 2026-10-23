/*
 * A174029  a(n) = 3*(3*n+1)*(5 - (-1)^n)/4.
 * https://oeis.org/A174029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(3*n+1)*(5 - (-1)^n)/4
 * Range:     u8 n <= 18, u16 n <= 4854, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_t = (seq_t)n;

    /* (-1)^n == 1 when n is even, == -1 when n is odd */
    if ((n & 1) == 0) {            /* n is even  -> a(2k) = 18k + 3 */
        return 3 * (3 * n_t + 1);
    } else {                       /* n is odd   -> a(2k+1) = 27k + 18 */
        seq_t k = n_t >> 1;        /* k = floor(n/2) */
        return 27 * k + 18;
    }
}
