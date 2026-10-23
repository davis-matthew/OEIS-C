/*
 * A102420  Number of partitions of n into exactly k = 5 parts with each part
 *          p <= 5.
 * https://oeis.org/A102420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of partitions of n into exactly 5 parts of size ≤
 *            5
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Outside the range of possible sums no such partition exists */
    if (n < 5 || n > 25) return 0;

    static int initialized = 0;
    static seq_t values[26] = {0};

    if (!initialized) {
        for (idx_t c1 = 0; c1 <= 5; ++c1)
            for (idx_t c2 = 0; c2 <= 5 - c1; ++c2)
                for (idx_t c3 = 0; c3 <= 5 - c1 - c2; ++c3)
                    for (idx_t c4 = 0; c4 <= 5 - c1 - c2 - c3; ++c4) {
                        idx_t c5 = 5 - c1 - c2 - c3 - c4;
                        idx_t sum = c1 + 2 * c2 + 3 * c3 + 4 * c4 + 5 * c5;
                        values[sum] += 1;
                    }
        initialized = 1;
    }

    return values[n];
}
