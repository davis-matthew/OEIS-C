/*
 * A044904  Numbers whose base-3 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 991, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   passes_base3_parity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑3 run lengths alternate odd, even, odd, … (first run odd). */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/passes_base3_parity.h"

/* Static cache for memoization. */
static seq_t *cache = NULL;
static size_t cacheSize = 0;
static size_t cacheCap = 0;
static seq_t lastChecked = 0;   /* Last integer that was tested. */

/* Ensure the cache array can hold at least `need` elements. */
static void ensure_capacity(size_t need)
{
    if (need <= cacheCap)
        return;
    size_t newCap = cacheCap ? cacheCap * 2 : 16;
    while (newCap < need)
        newCap *= 2;
    seq_t *newArr;
    if (cache == NULL)
        newArr = (seq_t *)malloc(newCap * sizeof(seq_t));
    else
        newArr = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
    cache = newArr;
    cacheCap = newCap;
}

/* Public function: compute a(n) with memoization. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n < 1 */

    if ((size_t)n <= cacheSize)
        return cache[n - 1];

    while (cacheSize < (size_t)n) {
        ++lastChecked;
        if (passes_base3_parity(lastChecked)) {
            ensure_capacity(cacheSize + 1);
            cache[cacheSize++] = lastChecked;
        }
    }
    return cache[n - 1];
}
