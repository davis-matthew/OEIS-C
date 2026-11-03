/*
 * A043206  Numbers k such that 4 and 5 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043206: numbers k such that 4 and 5 occur juxtaposed in the base‑9
   representation of k but not of k-1.  Computed by brute‑force scanning
   with memoisation. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Return true if the base‑9 representation of x contains the
   consecutive digits 4 and 5 in either order. */
static bool has_adjacent45(seq_t x)
{
    seq_t prev = -1;               /* sentinel value for "no previous digit" */
    while (x > 0) {
        seq_t d = x % 9;           /* least significant base‑9 digit */
        if (prev != -1) {
            if ((prev == 4 && d == 5) || (prev == 5 && d == 4))
                return true;
        }
        prev = d;
        x = x / 9;
    }
    return false;
}

/* Return the n-th element of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but safe placeholder */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static seq_t next_candidate = 1;

    while (cache_len < (size_t)n) {
        seq_t k = next_candidate++;
        if (has_adjacent45(k) && !has_adjacent45(k - 1)) {
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0; /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
    }
    return cache[n - 1];
}
