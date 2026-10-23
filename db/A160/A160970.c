/*
 * A160970  Indices of square numbers that are also 18-gonal numbers.
 * https://oeis.org/A160970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A160970 – Indices of square numbers that are also 18‑gonal numbers
   a(1) = 0
   a(2) = 1
   a(3) = 10
   a(4) = 44
   a(5) = 341
   a(n) = 34*a(n-2) - a(n-4)   for n > 5
*/

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache; cache[0] unused */
static idx_t cache_cap = 0;   /* allocated capacity (index of last usable slot) */
static idx_t computed = 0;    /* largest index that has been computed */

/* Ensure cache can hold indices up to n (inclusive).  The resize is done
   with a power‑of‑two growth strategy for amortized efficiency. */
static void ensure_cache_cap(idx_t n)
{
    if (cache_cap >= n)
        return;
    /* Grow to the next power of two >= n+1 */
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    /* realloc needs a cast in C++ */
    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) {
        abort();   /* out of memory */
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of sequence A160970. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; 0 is outside the defined range */
        return 0;

    if (n <= computed)
        return cache[n];

    ensure_cache_cap(n);

    /* Compute missing terms up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        switch (i) {
            case 1: cache[i] = 0;        break;
            case 2: cache[i] = 1;        break;
            case 3: cache[i] = 10;       break;
            case 4: cache[i] = 44;       break;
            case 5: cache[i] = 341;      break;
            default: /* n > 5 */
                cache[i] = 34 * cache[i - 2] - cache[i - 4];
                break;
        }
    }
    computed = n;
    return cache[n];
}
