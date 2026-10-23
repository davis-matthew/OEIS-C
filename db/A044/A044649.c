/*
 * A044649  Numbers k such that string 1,8 occurs in the base 9 representation
 *          of k but not of k+1.
 * https://oeis.org/A044649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has18_base9, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the nth integer k such that the base‑9 representation
   of k contains the substring 18 but the representation of k+1 does not. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/has18_base9.h"

/* The sequence function.  It uses a static cache so calls may occur in
   any order.  The array of results is grown only when needed. */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;   /* cached sequence values */
    static size_t  cap = 0;     /* allocated capacity */
    static size_t  len = 0;     /* number of cached values */
    static seq_t  last_k = 0;   /* last integer examined */

    /* OEIS offset is 1; behavior for n==0 is irrelevant */
    if (n == 0)
        return 0;

    /* Build the cache up to the requested index. */
    while (len < (size_t)n) {
        ++last_k; /* next candidate k */

        if (has18_base9(last_k) && !has18_base9(last_k + 1)) {
            /* enlarge cache if full */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 16 : (cap << 1);
                if (seq == NULL) {
                    seq = seq_alloc(new_cap);
                } else {
                    seq = seq_realloc(seq, cap, new_cap);
                }
                cap = new_cap;
            }
            seq[len++] = last_k;
        }
    }

    return seq[(size_t)(n - 1)];
}
