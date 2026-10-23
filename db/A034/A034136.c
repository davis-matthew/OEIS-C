/*
 * A034136  Number of partitions of n into distinct parts from [1, 6].
 * https://oeis.org/A034136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,3,4,5,6} whose sum is n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define MAX_SUM 21

static seq_t subset_counts[MAX_SUM + 1];
static bool initialized = false;

static void init_subset_counts(void)
{
    /* initialize counts to zero */
    for (size_t i = 0; i <= MAX_SUM; ++i)
        subset_counts[i] = 0;

    /* there are 2^6 = 64 subsets */
    for (uint8_t mask = 0; mask < 64; ++mask) {
        uint8_t sum = 0;
        for (uint8_t bit = 0; bit < 6; ++bit) {
            if (mask & (1 << bit))
                sum += (uint8_t)(bit + 1);   /* parts are 1..6 */
        }
        /* sum is in 0..21 */
        subset_counts[(size_t)sum] += 1;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_subset_counts();

    if (n <= (idx_t)MAX_SUM)
        return subset_counts[(size_t)n];
    else
        return (seq_t)0;
}
