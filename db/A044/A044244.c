/*
 * A044244  Numbers n such that string 7,1 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains71
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth integer whose base‑8 representation contains the
   substring "71" but whose predecessor n‑1 does not.  The function
   generates terms incrementally, memoising those already found, and
   checks the required condition by scanning the base‑8 digits of a
   number.  The algorithm is O(value_of(a(n))) which is far below the
   time limit for n≈10^4. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains71.h"

static seq_t *cache = NULL;     /* memoised terms, 0‑based index */
static size_t cache_cap = 0;     /* allocated capacity */
static size_t cache_len = 0;     /* number of terms stored */
static seq_t candidate = 0;      /* current number being examined */

static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap? cache_cap << 1 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                         /* not in sequence */
    if (n <= cache_len) return cache[n-1];

    /* generate terms until we reach the required index */
    while (cache_len < (size_t)n) {
        /* advance candidate until the defining property holds */
        do {
            candidate++;
        } while (!(contains71(candidate) && !contains71(candidate-1)));

        /* store the found term */
        ensure_capacity(cache_len + 1);
        cache[cache_len++] = candidate;
    }
    return cache[n-1];
}
