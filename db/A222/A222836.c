/*
 * A222836  Number of nX6 0..3 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..3 introduced
 *          in row major order.
 * https://oeis.org/A222836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+2*a(n-2)-4*a(n-3)+4*a(n-4)-4*a(n-5)+2*a(n-6)
 *            -6*a(n-7)+a(n-10) for n > 14 Initial values (1‑based): a(1) =
 *            55, a(2) = 1536, a(3) = 1944, a(4) = 4056, a(5) = 9600, a(6) =
 *            24576, a(7) = 55296, a(8) = 124416, a(9) = 279936, a(10) =
 *            645504, a(11) = 1476096, a(12) = 3393024, a(13) = 77...
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 20, u64 n <= 47, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N 20000   /* sufficient for the intended usage (≈10000) */

/* Static cache for the sequence; index 0 is unused. */
static seq_t cache[MAX_N + 1];
static idx_t computed_up_to = 0;
static bool initialized = false;

/* Initialise the first 14 terms of the cache. */
static void init_sequence(void)
{
    cache[0] = 0; /* unused */
    cache[1] = (seq_t)55;
    cache[2] = (seq_t)1536;
    cache[3] = (seq_t)1944;
    cache[4] = (seq_t)4056;
    cache[5] = (seq_t)9600;
    cache[6] = (seq_t)24576;
    cache[7] = (seq_t)55296;
    cache[8] = (seq_t)124416;
    cache[9] = (seq_t)279936;
    cache[10] = (seq_t)645504;
    cache[11] = (seq_t)1476096;
    cache[12] = (seq_t)3393024;
    cache[13] = (seq_t)7742976;
    cache[14] = (seq_t)17750400;
    computed_up_to = 14;
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, return 0 for safety */

    if (n > MAX_N)
        return 0; /* out of bounds for this implementation */

    if (!initialized)
        init_sequence();

    if (n <= computed_up_to)
        return cache[n];

    /* Compute missing terms up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i)
    {
        /* The recurrence requires indices up to i‑10; for i <= 14
           the initial values already exist, so we skip computation. */
        if (i <= 14)
            continue;

        seq_t val = 0;
        val += (seq_t)2 * cache[i - 1];
        val += (seq_t)2 * cache[i - 2];
        val -= (seq_t)4 * cache[i - 3];
        val += (seq_t)4 * cache[i - 4];
        val -= (seq_t)4 * cache[i - 5];
        val += (seq_t)2 * cache[i - 6];
        val -= (seq_t)6 * cache[i - 7];
        val += cache[i - 10];
        cache[i] = val;
    }

    computed_up_to = n;
    return cache[n];
}
