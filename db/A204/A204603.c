/*
 * A204603  Number of (n+2)X6 0..2 arrays with every 3X3 subblock having three
 *          strictly increasing elements in a row horizontally, vertically or
 *          nw-to-se diagonally exactly one way.
 * https://oeis.org/A204603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 17, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS sequence A204603
   Recurrence (for n > 15):
        a(n) = 442*a(n-3) + 43*a(n-6)
   with initial values a(1)..a(16) given below.  The function is
   memoised so that calls in any order remain fast.  It works for
   both 64‑bit unsigned integer builds and arbitrary‑precision
   integer builds compiled as C++17. */
#include <stdlib.h>

static const seq_t init_values[16] = {
    2275645ULL,
    4820300ULL,
    5260434ULL,
    10978524ULL,
    69792825ULL,
    774393122ULL,
    4518555426ULL,
    32448416953ULL,
    349259705982ULL,
    2010484777666ULL,
    14354494409473ULL,
    154409289786362ULL,
    888816408342414ULL,
    6346077122217149ULL,
    68263874184421262ULL,
    392943303332786626ULL
};

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    static seq_t *cache = NULL;          /* indices 1 .. cache_len-1 */
    static idx_t cache_len = 0;          /* number of allocated slots (index 0 unused) */

    /* enlarge the cache if necessary */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;            /* need indices up to n inclusive */

        /* realloc with explicit cast to satisfy C and C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failed */
        cache = tmp;

        /* initialise new entries to zero (will be overwritten) */
        for (idx_t i = cache_len; i < new_len; ++i) cache[i] = 0;

        /* fill the first 16 terms (if not already present) */
        for (idx_t i = 1; i <= 16 && i < new_len; ++i)
            cache[i] = init_values[i - 1];

        /* compute the recurrence for newly added indices */
        idx_t start = (cache_len > 16) ? cache_len : 17;
        for (idx_t i = start; i < new_len; ++i)
            cache[i] = cache[i - 3] * (seq_t)442 + cache[i - 6] * (seq_t)43;

        cache_len = new_len;
    }

    return cache[n];
}
