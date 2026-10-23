/*
 * A034139  Number of partitions of n into distinct parts from [1, 9].
 * https://oeis.org/A034139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts from 1..9
 *            (i.e. the number of subsets of {1,2,…,9} whose sum equals n).
 *            a(0) = 1, a(n) = 0 for n > 45. This is computed by a simple DP
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static seq_t cache[46];
static bool initialized = false;

static void init_cache(void)
{
    /* initialise dp array */
    for (int i = 0; i < 46; ++i)
        cache[i] = 0;
    cache[0] = 1; /* empty subset */

    /* iterate over each part 1..9 and update counts */
    for (int part = 1; part <= 9; ++part) {
        for (int sum = 45; sum >= part; --sum) {
            cache[sum] += cache[sum - part];
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized)
        init_cache();

    if (n <= 45)
        return cache[(size_t)n];
    return (seq_t)0;
}
