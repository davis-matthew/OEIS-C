/*
 * A037977  Numbers whose maximal base 4 run length is 2.
 * https://oeis.org/A037977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose maximal base 4 run length is exactly 2. The
 *            function returns the n-th such number (with a(1) = 5)
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   has_run_exact2, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_realloc.h"
#include "../utils/has_run_exact2.h"

static seq_t *cache   = NULL;          /* array of found numbers     */
static size_t  cap    = 0;             /* current allocated capacity */
static idx_t   len    = 0;             /* number of elements stored  */
static seq_t   cand   = 1;             /* smallest candidate to test */

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for n == 0, return 0 */
        return 0;

    /* Generate values up to index n if necessary. */
    while (len < n) {
        /* Find next candidate that satisfies the property */
        while (!has_run_exact2(cand))
            cand++;

        /* Ensure we have enough space to store it */
        if (len == cap) {
            size_t new_cap = (cap == 0) ? 4 : cap * 2;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        cache[len++] = cand;   /* store the found value */
        cand++;                /* next number to test */
    }

    return cache[n - 1];        /* a(1) is stored at index 0 */
}
