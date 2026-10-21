/*
 * A208015  Number of 5 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 23*a(n-3) + 6*a(n-4) + 6*a(n-5) -108*a(n-6) -
 *            72*a(n-7) + 216*a(n-9) for n > 11 with initial values: a(1)=12,
 *            a(2)=144, a(3)=420, a(4)=1943, a(5)=5952, a(6)=16224,
 *            a(7)=55624, a(8)=181192, a(9)=545140, a(10)=1737120,
 *            a(11)=5591900
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 36, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[11] = {
    12ULL,
    144ULL,
    420ULL,
    1943ULL,
    5952ULL,
    16224ULL,
    55624ULL,
    181192ULL,
    545140ULL,
    1737120ULL,
    5591900ULL
};

static seq_t *cache = NULL;   /* array of computed terms, 0‑based */
static size_t cap    = 0;     /* capacity of cache */
static size_t used   = 0;     /* number of terms already computed */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but return 0 for safety */

    /* Expand cache if required */
    if ((size_t)n > cap) {
        size_t new_cap = (cap == 0) ? ((size_t)n) : (cap << 1);
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* allocation failed – unlikely in tests */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms */
    for (size_t i = used + 1; i <= (size_t)n; ++i) {
        seq_t term;
        if (i <= 11) {
            term = init[i - 1];
        } else {
            /* recurrence, all operands are unsigned; the combination
               guarantees that the positive part exceeds the negative part,
               so subtraction is safe. */
            term = cache[i - 2];                                       /* a(n-1)  */
            term += (seq_t)23 * cache[i - 4];                         /* 23*a(n-3) */
            term += (seq_t)6 * cache[i - 5];                          /* 6*a(n-4)  */
            term += (seq_t)6 * cache[i - 6];                          /* 6*a(n-5)  */
            term -= (seq_t)108 * cache[i - 7];                        /* -108*a(n-6) */
            term -= (seq_t)72 * cache[i - 8];                         /* -72*a(n-7)  */
            term += (seq_t)216 * cache[i - 10];                       /* +216*a(n-9) */
        }
        cache[i - 1] = term;
    }
    used = (size_t)n;
    return cache[n - 1];
}
