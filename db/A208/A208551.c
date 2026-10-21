/*
 * A208551  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) -10*a(n-3) - 5*a(n-4) +20*a(n-5) *
 *            -20*a(n-7) + 5*a(n-8) +10*a(n-9) -4*a(n-10) - 2*a(n-11) +
 *            a(n-12) * (for n ≥ 13; a(1)…a(12) are given below)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 77, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build supplies typedefs for idx_t and seq_t.  We only use them
 * here.  No other types are redeclared. */
#include <stdlib.h>

/* memoisation cache; index 0 is unused for simplicity */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* enlarge the cache to contain at least indices 0…n */
static void grow_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = n + 16;                     /* some spare room */
        seq_t *tmp = (seq_t *)realloc(cache,
                                       (size_t)new_size * sizeof(seq_t));
        if (!tmp) {
            abort();                                 /* allocation failure */
        }
        cache = tmp;
        cache_size = new_size;
    }
}

/* initialise the first 12 terms of the sequence */
static void init_cache(void)
{
    if (cache_size == 0) {
        grow_cache(12);
        cache[0] = (seq_t)0;                          /* unused */
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)100;
        cache[3] = (seq_t)330;
        cache[4] = (seq_t)1089;
        cache[5] = (seq_t)2508;
        cache[6] = (seq_t)5776;
        cache[7] = (seq_t)11020;
        cache[8] = (seq_t)21025;
        cache[9] = (seq_t)35670;
        cache[10] = (seq_t)60516;
        cache[11] = (seq_t)94710;
        cache[12] = (seq_t)148225;
    }
}

/* Return the n‑th term of A208551 (n≥1).  Values for n<1 are undefined
 * and return 0. */
seq_t a(idx_t n)
{
    if (n < 1) {
        return (seq_t)0;
    }

    init_cache();                /* guarantees cache[1…12] are set */
    grow_cache(n);               /* ensure cache large enough */

    if (n <= 12) {
        return cache[n];
    }

    /* compute successive terms using the recurrence */
    for (idx_t i = 13; i <= n; ++i) {
        seq_t pos = cache[i-1] * (seq_t)2
                  + cache[i-2] * (seq_t)4
                  + cache[i-5] * (seq_t)20
                  + cache[i-8] * (seq_t)5
                  + cache[i-9] * (seq_t)10
                  + cache[i-12];
        seq_t sub = cache[i-3] * (seq_t)10
                  + cache[i-4] * (seq_t)5
                  + cache[i-7] * (seq_t)20
                  + cache[i-10] * (seq_t)4
                  + cache[i-11] * (seq_t)2;
        cache[i] = pos - sub;
    }

    return cache[n];
}
