/*
 * A034137  Number of partitions of n into distinct parts from [1, 7].
 * https://oeis.org/A034137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1,2,3,4,5,6,7} that sum to n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to partitions of n into distinct parts from [1,7] */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Static count array for sums 0..28.  Lazy initialization is performed once. */
static seq_t counts[29];
static int initialized = 0;

/* Compute the subset‑sum counts once. */
static void init_counts(void)
{
    if (initialized) return;
    for (int mask = 0; mask < 128; ++mask) {        /* 2^7 subsets */
        int sum = 0;
        for (int bit = 0; bit < 7; ++bit) {
            if (mask & (1 << bit))
                sum += bit + 1;                     /* part values are 1..7 */
        }
        if (sum <= 28)
            counts[sum] = counts[sum] + 1;          /* ++ counts[sum] in seq_t */
    }
    initialized = 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_counts();
    if (n > 28)   /* maximum possible sum with distinct parts 1..7 */
        return 0;
    return counts[(size_t)n];
}
