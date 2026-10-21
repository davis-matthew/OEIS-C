/*
 * A054550  Composite numbers whose least prime factor is either 5 or 7.
 * https://oeis.org/A054550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054550: composite numbers whose least prime factor is
   5 or 7 (offset 1).  
   The implementation enumerates integers in ascending order, checks
   that the number is composite and that its smallest prime divisor is
   5 or 7, and memoises the results so that any call order is fast. */
/* Dependencies: standard <stdlib.h> for dynamic memory, <stdint.h> for
   types.  The types `idx_t` and `seq_t` are supplied by the build. */

#include <stdlib.h>
#include <stdint.h>

/* external integer types supplied by the build */
typedef uint64_t idx_t;          /* index type (unsigned) */

/* ---------------------------------------------------------------------- */
/*  a(idx_t n)  --  return the n-th term (n ≥ 1).  */
seq_t
a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t sz = 0;         /* number of terms stored */
    static size_t cap = 0;        /* allocated capacity */
    static uint64_t last_checked = 4;   /* last integer examined (≥ 4) */

    /* Guard against invalid small n; behaviour below the offset is
       irrelevant per the requirements. */
    if (n == 0) return (seq_t)0;

    /* Extend cache if we need more terms. */
    if (n > sz) {
        /* Allocate enough space for at least n terms. */
        if (n > cap) {
            size_t new_cap = (cap > 0) ? cap * 2 : 4;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();      /* out of memory */
            cache = tmp;
            cap = new_cap;
        }

        /* Generate terms until we have n of them. */
        for (; sz < n; ++sz) {
            uint64_t cand = last_checked + 1;

            /* Find the next composite whose least prime factor
               is 5 or 7.  Skip candidates that are < 8 (since 5 and 7
               themselves are prime). */
            while (1) {
                /* Skip 1–7 explicitly: they are not composite. */
                if (cand <= 7) {
                    ++cand;
                    continue;
                }

                /* If divisible by 2 or 3, the least prime factor would be
                   2 or 3, so reject. */
                if ((cand & 1) == 0) {       /* cand % 2 == 0 */
                    ++cand;
                    continue;
                }
                if ((cand % 3) == 0) {       /* cand % 3 == 0 */
                    ++cand;
                    continue;
                }

                /* Now check divisibility by 5 and 7. */
                if ((cand % 5) == 0) {
                    /* Number divisible by 5; lpf is 5 (since no smaller
                       prime divides it). Accept it. */
                    break;
                }
                if ((cand % 7) == 0) {
                    /* Number divisible by 7; ensure it is not divisible by
                       5 (already checked), and it is composite because
                       it has at least one prime factor (7). Accept. */
                    break;
                }
                /* Not divisible by 5 or 7; skip. */
                ++cand;
            }

            cache[sz] = (seq_t)cand;
            last_checked = cand;
        }
    }

    return cache[n - 1];
}
