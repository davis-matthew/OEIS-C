/*
 * A044885  Numbers having, in base 14, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth integer whose base‑14 run‑length parity sums are
 *            equal
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   has_equal_parity_run_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is defined by looking at the base‑14 representation of an
    integer.  The digits are grouped into consecutive runs (each run is a
    maximal block of equal digits).  Let S_even be the sum of the lengths of
    those runs whose length is even and let S_odd be the sum of the lengths
    of the runs whose length is odd.  An integer a satisfies the property if
    S_even = S_odd.  a(1) is the first such integer (2745).  The function
    below returns the nth element of this sequence.  It maintains a static
    cache so that calls are efficient for any order of n.  The implementation
    works for all supported seq_t and idx_t types. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_equal_parity_run_sum.h"

static seq_t *cache        = NULL;   /* cached sequence values */
static size_t cache_cap    = 0;      /* allocated cache capacity */
static idx_t   cache_len   = 0;      /* number of values stored */
static seq_t   next_candidate;       /* next integer to test */
static bool    initialized   = false;/* first call flag */

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;           /* undefined, but safe */

    if (!initialized) {
        initialized   = true;
        next_candidate = (seq_t)2745;     /* first element of the sequence */
    }

    /* Generate terms until we have at least n terms cached */
    while (cache_len < n) {
        if (has_equal_parity_run_sum(next_candidate)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return (seq_t)0;  /* allocation failure */
                cache      = tmp;
                cache_cap  = new_cap;
            }
            cache[cache_len++] = next_candidate;
        }
        next_candidate += (seq_t)1;
    }

    return cache[n - 1];
}
