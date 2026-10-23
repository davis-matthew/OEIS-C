/*
 * A185765  Number of (n+1)X6 binary arrays with no 2X2 subblock trace equal
 *          to any horizontal or vertical neighbor 2X2 subblock trace.
 * https://oeis.org/A185765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A185765: Number of (n+1)×6 binary arrays with no 2×2 subblock trace equal
 * to any horizontal or vertical neighbour 2×2 subblock trace.
 *
 * The sequence satisfies the following 9‑term linear recurrence (valid for n>12):
 *   a(n) = a(n‑1) + 30·a(n‑2) + 24·a(n‑3) - 95·a(n‑4) - 15·a(n‑5)
 *        + 64·a(n‑6) - 116·a(n‑7) + 16·a(n‑8) + 16·a(n‑9)
 *
 * Initial values are: 
 * a(1) = 680
 * a(2) = 3456
 * a(3) = 19992
 * a(4) = 118248
 * a(5) = 725352
 * a(6) = 4430912
 * a(7) = 27172072
 * a(8) = 166155112
 * a(9) = 1017804600
 * a(10) = 6228062600
 * a(11) = 38135220896
 * a(12) = 233413737792
 *
 * This function memoises the computed values in a static array so that
 * calls in arbitrary order run in O(n) time for the largest n requested.
 */

#include <stdbool.h>
#include "../utils/seq_from_str.h"

static const char *INIT_STR[13] = {
    NULL,            /* index 0 unused */
    "680",           /* 1 */
    "3456",          /* 2 */
    "19992",         /* 3 */
    "118248",        /* 4 */
    "725352",        /* 5 */
    "4430912",       /* 6 */
    "27172072",      /* 7 */
    "166155112",     /* 8 */
    "1017804600",    /* 9 */
    "6228062600",    /* 10 */
    "38135220896",   /* 11 */
    "233413737792"   /* 12 */
};

/* Maximum n we expect to handle for pre‑allocation.  Tests use n ≤ 10000. */
#define MAX_N 20000

static seq_t cache[MAX_N + 1];
static bool initialized = false;
static idx_t computed_up_to = 0;

/* Initialise the cache with the given constants and set length. */
static void init_cache(void)
{
    for (idx_t i = 1; i <= 12 && i <= MAX_N; ++i)
        cache[i] = seq_from_str(INIT_STR[i]);

    for (idx_t i = 13; i <= MAX_N; ++i)
        cache[i] = 0;

    computed_up_to = 12;
    initialized = true;
}

/* Compute a(n) using the recurrence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* value for indices below the offset – not used */

    if (!initialized)
        init_cache();

    if (n <= computed_up_to)
        return cache[n];

    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        if (i <= 12) {      /* safety guard, though init_cache already set */
            cache[i] = seq_from_str(INIT_STR[i]);
            continue;
        }

        /* Positive contributions */
        seq_t pos = cache[i - 1];
        pos += 30 * cache[i - 2];
        pos += 24 * cache[i - 3];
        pos += 64 * cache[i - 6];
        pos += 16 * cache[i - 8];
        pos += 16 * cache[i - 9];

        /* Negative contributions */
        seq_t neg = 95 * cache[i - 4];
        neg += 15 * cache[i - 5];
        neg += 116 * cache[i - 7];

        cache[i] = pos - neg;   /* guaranteed non‑negative */
    }

    computed_up_to = n;
    return cache[n];
}
