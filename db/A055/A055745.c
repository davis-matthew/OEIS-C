/*
 * A055745  Squarefree numbers not of form ab + bc + ca for 1 <= a <= b <= c
 *          (probably the list is complete).
 * https://oeis.org/A055745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, hard, nice
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-03)
 * Helpers:   is_squarefree, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A055745: squarefree numbers not of the form ab+bc+ca for 1 ≤ a ≤ b ≤ c
   This implementation searches for squarefree numbers and tests whether
   they can be expressed as ab+bc+ca.  It caches found terms so that
   multiple calls are efficient.  The logic is independent of the
   representation size of seq_t. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"
#include "../utils/mem.h"

/* Test whether a squarefree number is representable as ab + bc + ca
   with 1 ≤ a ≤ b ≤ c.  The loops are bounded by the obvious limits
   derived from the divisibility conditions. */
static bool has_representation(seq_t x)
{
    for (idx_t a = 1; (seq_t)a * (seq_t)a <= x; ++a) {
        seq_t as = (seq_t)a;
        seq_t max_b = as == 0 ? 0 : x / as;
        for (idx_t b = a; (seq_t)b * (seq_t)b <= max_b; ++b) {
            seq_t ab = as * (seq_t)b;
            seq_t d  = as + (seq_t)b;
            if (x < ab) break;
            seq_t num = x - ab;
            if (num % d != 0) continue;
            seq_t c = num / d;
            if (c >= (seq_t)b) return true;
        }
    }
    return false;
}

/* Main sequence function.  It maintains a static cache so that
   repeated calls do not recompute the entire search. */
seq_t a(idx_t n)
{
    static seq_t *seq_arr  = NULL;
    static size_t seq_len  = 0;
    static size_t seq_cap  = 0;
    static idx_t  cur      = 1;    /* next candidate to test */

    /* Grow the cache until it contains at least n terms. */
    while (seq_len < n) {
        /* Find the next squarefree number not representable. */
        while (true) {
            if (is_squarefree(cur) && !has_representation(cur)) {
                /* Grow storage if necessary. */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap == 0 ? 64 : seq_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, seq_arr, new_cap);
                    if (!tmp) return (seq_t)0;          /* allocation failure */
                    seq_arr = tmp;
                    seq_cap = new_cap;
                }
                seq_arr[seq_len++] = cur;
                ++cur;                /* advance to next candidate */
                break;                /* found next sequence term */
            }
            ++cur;                    /* try next number */
        }
    }

    return seq_arr[n - 1];
}
