/*
 * A165271  a(n) = p(3*n), where p(n) = number of partitions of n into parts
 *          that correspond to areas of distinct pieces of Archimedes'
 *          ostomachion.
 * https://oeis.org/A165271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of subsets of the 14 distinct Ostomachion piece
 *            areas (3,3,6,6,6,6,9,12,12,12,12,12,21,24) that sum to 3·n
 * Range:     u8 n <= 11, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The 14 piece areas (duplicates are allowed because pieces are distinct). */
    static const int areas[14] = {
        3, 3, 6, 6, 6, 6, 9,
        12, 12, 12, 12, 12,
        21, 24
    };

    /* Count of subsets achieving each possible sum (0 … 144). */
    static seq_t sumCnt[145] = {0};
    static bool initialized = false;

    if (!initialized) {
        /* Enumerate all 2^14 subsets. */
        const size_t totalMasks = (size_t)1 << 14;
        for (size_t mask = 0; mask < totalMasks; ++mask) {
            int sum = 0;
            for (int i = 0; i < 14; ++i) {
                if (mask & ((size_t)1 << i))
                    sum += areas[i];
            }
            ++sumCnt[(size_t)sum];
        }
        initialized = true;
    }

    /* For any n > 48, 3*n > 144 so no subset can reach that sum. */
    if (n > (idx_t)48)
        return (seq_t)0;

    /* 3 * n is safe to compute here (n <= 48). */
    idx_t target = n * ((idx_t)3);

    return sumCnt[(size_t)target];
}
