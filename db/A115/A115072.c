/*
 * A115072  Numbers with no "e" in Dutch (Miljard without "een").
 * https://oeis.org/A115072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   a(n) = sequence of integers whose Dutch spelling contains no
 *            letter 'e' (0, 5, 8, 12, 20, 50, 80, 1 000 000 000, 5 000 000
 *            000, 8 000 000 000, 12 000 000 000, 20 000 000 000, 50 000 000
 *            000, 80 000 000 000, …)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 8, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1)                      /* a(1) = 0 */
        return (seq_t)0;

    /* first block (k = 0) – 6 numbers */
    if (n <= 7) {                    /* indices 2..7 */
        static const idx_t block0[] = {5, 8, 12, 20, 50, 80};
        return (seq_t)block0[n - 2];
    }

    /* subsequent blocks (k >= 1) – 7 numbers per block */
    idx_t pos = n - 2 - 6;           /* zero‑based position after block0 */
    idx_t block = pos / 7;           /* which 10^9‑block (0 => 10^9) */
    idx_t idx   = pos % 7;           /* offset within the block   */

    static const idx_t block1[] = {1, 5, 8, 12, 20, 50, 80};
    idx_t d = block1[idx];           /* multiplier digit (1,5,…) */

    /* compute 10^(9*(block+1))  */
    seq_t pow10_9k = pow10((idx_t)(9ULL * (block + 1)));

    return (seq_t)d * pow10_9k;
}
