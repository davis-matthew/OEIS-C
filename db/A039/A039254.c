/*
 * A039254  Numbers whose base-12 representation has the same number of 5's
 *          and 6's.
 * https://oeis.org/A039254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer (starting from 0) whose base‑12
 *            representation * contains the same number of digits 5 as digits
 *            6
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   balanced_in_base12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                                                                       */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/balanced_in_base12.h"

/* Cached sequence storage */
static seq_t *seq_cache     = NULL;
static size_t seq_len       = 0;
static size_t seq_cap       = 0;

static seq_t next_candidate = 0;          /* next integer to test */

/* Ensure the cache array has enough room for at least one more element. */
static void ensure_cache_capacity(void)
{
    if (seq_len < seq_cap) return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_t *new_buf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_buf) /* memory failure: abort (no I/O allowed) */
        return;

    seq_cache = new_buf;
    seq_cap   = new_cap;
}

/* Sequence a(n) for n >= 1.  If n == 0 the function returns 0, as the
 * behaviour is unspecified below the experimental offset. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; behaviour for n==0 not required */

    /* Generate numbers until we have at least n terms. */
    while (seq_len < (size_t)n)
    {
        if (balanced_in_base12(next_candidate))
        {
            ensure_cache_capacity();
            seq_cache[seq_len++] = next_candidate;
        }
        next_candidate++;
    }
    return seq_cache[(size_t)(n - 1)];
}
