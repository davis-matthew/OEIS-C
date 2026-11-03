/*
 * A224759  a(2)=8; thereafter a(n) = smallest number m such that a(n-1)+m =
 *          (a(n-1) followed by the leading digit of m).
 * https://oeis.org/A224759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-03)
 * Helpers:   mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=8; for n>1, a(n) is the smallest m such that
   a(n-1)+m = concat(a(n-1), leading digit of m).  That is,
   a(n)=9*a(n-1)+d where d is the leading digit of a(n).  The
   implementation chooses the minimal d∈{1..9} satisfying the
   leading‑digit condition. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t   len     = 0;   /* number of terms stored (≥0) */
    static idx_t   cap     = 0;   /* allocated capacity */

    /* initialise cache with the first term if empty */
    if (len == 0) {
        cache = mem_new(seq_t, 1);
        if (!cache) return 0;          /* allocation failure */
        cache[0] = 8;                  /* a(1) */
        len = 1;
        cap = 1;
    }

    /* if requested index already computed, return it */
    if (n <= len) {
        return cache[n - 1];
    }

    /* grow cache if necessary */
    if (n > cap) {
        seq_t *tmp = mem_resize(seq_t, cache, n);
        if (!tmp) {
            mem_free(cache);
            cache = NULL;
            len = 0;
            cap = 0;
            return 0;                  /* allocation failure */
        }
        cache = tmp;
        cap = n;
    }

    /* compute successive terms up to n */
    for (idx_t i = len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t next = 0;

        for (int d = 1; d <= 9; ++d) {
            seq_t cand = prev * (seq_t)9 + (seq_t)d;
            /* determine leading digit of cand */
            size_t   digits   = num_digits(cand);
            seq_t    p10      = pow10((idx_t)(digits - 1)); /* 10^(digits-1) */
            seq_t    lead     = cand / p10;
            if (lead == (seq_t)d) {
                next = cand;
                break;
            }
        }

        /* If no candidate found (should not happen), fallback to zero */
        cache[i] = next;
    }

    len = n;
    return cache[n - 1];
}
