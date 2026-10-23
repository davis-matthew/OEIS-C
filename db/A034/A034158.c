/*
 * A034158  Number of partitions of n into 3 distinct parts from [ 1,15 ].
 * https://oeis.org/A034158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 3 distinct parts from
 *            [1,15]
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by brute‐force enumeration of all triples 1<=i<j<k<=15. */

seq_t a(idx_t n)
{
    /* The maximum possible sum is 13+14+15 = 42. */
    if (n > 42) return 0;

    seq_t count = 0;
    for (idx_t i = 1; i <= 13; ++i) {
        for (idx_t j = i + 1; j <= 14; ++j) {
            for (idx_t k = j + 1; k <= 15; ++k) {
                seq_t sum = (seq_t)i + (seq_t)j + (seq_t)k;
                if (sum == (seq_t)n)
                    ++count;
            }
        }
    }

    return count;
}
