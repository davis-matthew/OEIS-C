/*
 * A089742  Number of subwords UHH...HD in all peakless Motzkin paths of
 *          length n+3, where U=(1,1), D=(1,-1) and H=(1,0).
 * https://oeis.org/A089742
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A089742 – Number of subwords UHH…HD in all peakless Motzkin paths of length n+3
    Recurrence (n ≥ 7):
        (n+2)*a(n) =  (4n+5)*a(n‑1) – (5n‑1)*a(n‑2) + (5n‑7)*a(n‑3)
                      – (5n‑3)*a(n‑4) + (5n‑9)*a(n‑5) – (4n‑13)*a(n‑6)
                      + (n‑4)*a(n‑7)
    Initial values:
        a(0)=1, a(1)=3, a(2)=7, a(3)=17, a(4)=41, a(5)=99, a(6)=242
*/

#include <stdint.h>
#include <stdlib.h>

/* maximum n we will support (about 10000 is enough for the tests) */
#define MAX_N 20000

static seq_t cache[MAX_N + 1];    /* memoisation table, zero‑initialised */
static size_t cache_filled = 0;   /* first index that has not yet been set */

/* initialise the cache with the known values a(0)…a(6) */
static void init_cache(void)
{
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)17;
    cache[4] = (seq_t)41;
    cache[5] = (seq_t)99;
    cache[6] = (seq_t)242;
    cache_filled = 7;      /* next index to compute is 7 */
}

/* Main function */
seq_t a(idx_t n)
{
    if (n > MAX_N) abort();      /* safeguard – sequence defined for n≥0 */

    if (cache_filled == 0)       /* first call: set up the initial values */
        init_cache();

    /* if the value is already known, return it */
    if ((size_t)n < cache_filled)
        return cache[(size_t)n];

    /* compute terms from cache_filled up to n */
    for (size_t k = cache_filled; k <= (size_t)n; ++k)
    {
        /* recurrence only applies for k ≥ 7 */
        seq_t sum = (seq_t)0;
        sum += cache[k - 1] * (seq_t)(4 * k + 5);
        sum -= cache[k - 2] * (seq_t)(5 * k - 1);
        sum += cache[k - 3] * (seq_t)(5 * k - 7);
        sum -= cache[k - 4] * (seq_t)(5 * k - 3);
        sum += cache[k - 5] * (seq_t)(5 * k - 9);
        sum -= cache[k - 6] * (seq_t)(4 * k - 13);
        sum += cache[k - 7] * (seq_t)(k - 4);
        cache[k] = sum / (seq_t)(k + 2);
    }
    cache_filled = n + 1;
    return cache[(size_t)n];
}
