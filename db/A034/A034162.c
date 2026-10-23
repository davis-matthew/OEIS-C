/*
 * A034162  Number of partitions of n into 3 distinct parts from [ 1,19 ].
 * https://oeis.org/A034162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into 3 distinct parts from [1,19]
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The maximum possible sum is 19 + 18 + 17 = 54. */
    if (n > 54U) return 0;

    seq_t target = (seq_t)n;
    seq_t count  = 0;

    for (idx_t i = 1; i <= 17; ++i) {
        for (idx_t j = i + 1; j <= 18; ++j) {
            for (idx_t k = j + 1; k <= 19; ++k) {
                if ((seq_t)i + j + k == target) {
                    ++count;
                }
            }
        }
    }

    return count;
}
