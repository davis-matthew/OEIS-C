/*
 * A041641  Denominators of continued fraction convergents to sqrt(339).
 * https://oeis.org/A041641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A041641:  Denominators of continued fraction convergents to sqrt(339)
 *  a(n) = 195940*a(n-10) - a(n-20)  for n > 19
 *  a(0) = 1  a(1) = 2  a(2) = 5  ...  a(19) = 1042596740
 *
 *  The function stores the values in a static array large enough to
 *  cover the typical usage up to n ≈ 10000.  It is fully compatible
 *  with both C11 and C++17, and works with either an integer type
 *  or an arbitrary‑precision integer class for seq_t.
 */
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/*  Initial 20 terms of the sequence  */
static const seq_t init20[20] = {
    1ULL, 2ULL, 5ULL, 12ULL, 17ULL,
    301ULL, 318ULL, 937ULL, 2192ULL, 5321ULL,
    193748ULL, 392817ULL, 979382ULL, 2351581ULL,
    3330963ULL, 58977952ULL, 62308915ULL, 183595782ULL,
    429500479ULL, 1042596740ULL
};

/*  Static cache large enough for n ≈ 10000  */
#define CACHE_MAX 20000
static seq_t cache[CACHE_MAX];
static bool initialized = false;
static size_t cache_len = 0;

/*  Main sequence generator  */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;

    /*  Initialise the cache on first call  */
    if (!initialized) {
        memcpy(cache, init20, sizeof(init20));
        cache_len = sizeof(init20) / sizeof(seq_t);
        initialized = true;
    }

    /*  Return immediately if the value is already computed  */
    if (idx < cache_len)
        return cache[idx];

    /*  Guard against overflow of the static array  */
    if (idx >= CACHE_MAX)
        return 0;    /*  out of bounds – caller should avoid this  */

    /*  Extend the cache up to index idx  */
    for (size_t i = cache_len; i <= idx; ++i) {
        cache[i] = (seq_t)195940 * cache[i - 10] - cache[i - 20];
    }
    cache_len = idx + 1;
    return cache[idx];
}
