/*
 * A044909  Numbers whose base-8 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑8 run lengths
 *            alternate odd, even, odd, ... (starting with an odd run)
 * Range:     u8 n <= 31, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   parity_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/parity_ok.h"

/* Allocate and store terms progressively; keeps results for future
   calls regardless of order. */
static seq_t *s_cache = NULL;   /* array of computed terms   */
static idx_t  s_len   = 0;       /* number of terms stored   */
static idx_t  s_cap   = 0;       /* current cache capacity   */
static seq_t  s_next  = 1;       /* next candidate number    */

seq_t a(idx_t n)
{
    /* Extend cache until it contains at least n terms */
    while (s_len < n) {
        /* Grow storage if necessary */
        if (s_len == s_cap) {
            idx_t new_cap = s_cap ? s_cap << 1 : (idx_t)16;
            s_cache = (seq_t *)realloc(s_cache, new_cap * sizeof(seq_t));
            s_cap = new_cap;
        }

        if (parity_ok(s_next)) {
            s_cache[s_len++] = s_next;
        }
        ++s_next;
    }
    return s_cache[n - 1];
}
