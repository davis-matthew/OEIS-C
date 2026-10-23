/*
 * A142883  a(n) = A142590(n)/3.
 * https://oeis.org/A142883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4k) = 3k^2 + k * a(4k+1) = 48k^2 + 40k + 7 * a(4k+2) = 12k^2 +
 *            16k + 5 * a(4k+3) = 48k^2 + 88k + 39
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    switch (n & 3ULL)
    {
        case 0: {
            idx_t k = n >> 2;   /* n = 4k */
            return (seq_t)3 * k * k + k;
        }
        case 1: {
            idx_t k = (n - 1) >> 2;   /* n = 4k+1 */
            return (seq_t)48 * k * k + 40 * k + 7;
        }
        case 2: {
            idx_t k = (n - 2) >> 2;   /* n = 4k+2 */
            return (seq_t)12 * k * k + 16 * k + 5;
        }
        case 3: {
            idx_t k = (n - 3) >> 2;   /* n = 4k+3 */
            return (seq_t)48 * k * k + 88 * k + 39;
        }
    }
    return 0; /* unreachable */
}
