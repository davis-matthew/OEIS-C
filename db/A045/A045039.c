/*
 * A045039  Numbers whose base-4 representation contains no 0's and exactly
 *          two 2's.
 * https://oeis.org/A045039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 2815, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A045039 – numbers whose base‑4 representation contains
   no 0's and exactly two 2's.  The function enumerates base‑4
   strings of increasing length, using only digits 1,2,3, and keeps
   track of the remaining number of 2's to place.  All valid numbers
   are generated in ascending numeric order and cached for repeated
   calls.  The cache grows lazily as larger indices are requested. */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;       /* cached terms               */
static size_t cacheSize = 0;      /* number of stored terms      */
static size_t cacheCap  = 0;      /* current capacity            */
static idx_t  nextLenToGenerate = 2;   /* next length to generate   */

/* Ensure that 'cache' can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cacheCap < need) {
        size_t newCap = cacheCap ? cacheCap * 2 : 8;
        if (newCap < need) newCap = need;
        cache = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        cacheCap = newCap;
    }
}

/* Recursive enumeration of base‑4 strings of length L.
   pos   : current position (0‑based)
   L     : total length
   twos  : number of 2's still to place
   value : numeric value accumulated so far */
static void gen_rec(idx_t pos, idx_t L, int twos, seq_t value)
{
    /* Not enough positions left to place remaining 2's */
    if (twos > (int)(L - pos)) return;

    if (pos == L) {
        if (twos == 0) {
            ensure_cache_capacity(cacheSize + 1);
            cache[cacheSize++] = value;
        }
        return;
    }

    /* Choose digit 1 */
    gen_rec(pos + 1, L, twos, value * 4 + 1);

    /* Choose digit 2 if needed */
    if (twos > 0) {
        gen_rec(pos + 1, L, twos - 1, value * 4 + 2);
    }

    /* Choose digit 3 */
    gen_rec(pos + 1, L, twos, value * 4 + 3);
}

/* Generate all terms of a particular length L
   (exactly two 2's, digits 1,2,3, no zeros). */
static void generate_length(idx_t L)
{
    gen_rec(0, L, 2, 0);
}

/* External API: return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n==0, guard */

    /* Return cached term if available */
    if (cacheSize >= n) return cache[n - 1];

    /* Generate terms until we reach index n */
    while (cacheSize < n) {
        generate_length(nextLenToGenerate);
        nextLenToGenerate++;    /* proceed to next length */
    }
    return cache[n - 1];
}
