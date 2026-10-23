/*
 * A043949  Numbers k such that 2 and 7 occur juxtaposed in the base-8
 *          representation of k but not of k+1.
 * https://oeis.org/A043949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent27
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements search for numbers k such that the base‑8 representation of k
   contains the digits 2 and 7 adjacent in either order, and the
   representation of k+1 does not.  The function caches previously found
   terms so that calls are O(1) after an initial scan. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent27.h"

static seq_t *cache = NULL;
static size_t cap = 0;
static idx_t count = 0;
static seq_t last_k = 0;

static void grow(size_t need)
{
    if (need <= cap)
        return;
    size_t ncap = cap < 4 ? 4 : cap * 2;
    while (ncap < need)
        ncap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, ncap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cap = ncap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* safety; the sequence is offset 1 */

    /* Return cached result if already computed */
    if (n <= count)
        return cache[n - 1];

    /* Generate terms until we reach the requested n */
    while (count < n) {
        seq_t k = last_k + 1;
        for (;;) {
            if (has_adjacent27(k) && !has_adjacent27(k + 1)) {
                grow(count + 1);
                cache[count++] = k;
                last_k = k;
                break;
            }
            ++k;
        }
    }
    return cache[n - 1];
}
