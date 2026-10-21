/*
 * A072176  Unimodal analog of Fibonacci numbers: a(n+1) =
 *          Sum_{k=0..floor(n/2)} A071922(n-k,k).
 * https://oeis.org/A072176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-5) - (2*a(n-3) + a(n-4)) with
 *            initial values a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5. This
 *            recurrence follows from the OEIS definition for A072176. The
 *            implementation uses memoisation and O(n) time per largest
 *            requested n. All arithmetic is performed with the native...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 37, u64 n <= 71, u128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of entries currently stored
                                       (size_t because realloc expects it) */

    /* Convert idx_t to size_t safely – n is positive and small enough. */
    size_t N = (size_t)n;

    /* Ensure the cache has room for indices 1..N. */
    if (N + 1 > cache_size) {
        size_t new_size = N + 1;          /* +1 since we ignore index 0 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – in a contest environment this would
               terminate the program.  Here we just return 0. */
            return (seq_t)0;
        }
        cache = new_cache;
        /* Initialise any new entries to 0 so that additions are safe. */
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* Base cases – indices 1..5.  Store them on first use. */
    if (N >= 1 && cache[1] == 0 && N >= 1) {
        cache[1] = (seq_t)1;
    }
    if (N >= 2 && cache[2] == 0 && N >= 2) {
        cache[2] = (seq_t)1;
    }
    if (N >= 3 && cache[3] == 0 && N >= 3) {
        cache[3] = (seq_t)2;
    }
    if (N >= 4 && cache[4] == 0 && N >= 4) {
        cache[4] = (seq_t)3;
    }
    if (N >= 5 && cache[5] == 0 && N >= 5) {
        cache[5] = (seq_t)5;
    }

    /* Compute terms up to N if not already computed. */
    for (size_t i = 6; i <= N; ++i) {
        if (cache[i] != 0) continue;      /* Already computed previously */

        seq_t val = cache[i-1];  /* cache[i-1] */
        val = val + val;          /* 2*cache[i-1] */
        val = val + cache[i-2];   /* + cache[i-2] */
        val = val + cache[i-5];   /* + cache[i-5] */

        seq_t sub = cache[i-3];
        sub = sub + sub;          /* 2*cache[i-3] */
        sub = sub + cache[i-4];   /* + cache[i-4] */

        val = val - sub;          /* subtract the lower sum */

        cache[i] = val;
    }

    return cache[N];
}
