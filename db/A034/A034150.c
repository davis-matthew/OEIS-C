/*
 * A034150  Number of partitions of n into distinct parts from [1, 20].
 * https://oeis.org/A034150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the number of partitions of n into distinct parts
   from the set {1,2,…,20}.  For n > 210 the result is 0 because the sum
   of all 20 parts is 210.  The implementation performs a standard
   subset‑sum dynamic‑programming calculation which is memoised in a static
   array so that repeated calls are O(1).  The algorithm runs in
   O(20·210) ≈ 4 200 operations and uses at most 211 elements of type
   seq_t. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Array holds a(k) for 0 ≤ k ≤ 210 */
    static seq_t cache[211] = { 0 };
    static int initialized = 0;

    /* No computation needed for n > 210 */
    if (n > 210) {
        return (seq_t)0;
    }

    if (!initialized) {
        /* Initialise DP table for all sums up to 210 */
        cache[0] = (seq_t)1;
        for (idx_t part = 1; part <= 20; ++part) {
            for (size_t sum = 210; sum >= part; --sum) {
                cache[sum] += cache[sum - part];
            }
        }
        initialized = 1;
    }

    return cache[(size_t)n];
}
