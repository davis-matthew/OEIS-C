/*
 * A050088  a(n) = floor(a(n-1)/3) if this is positive and not yet in the
 *          sequence, otherwise a(n) = 6*a(n-1).
 * https://oeis.org/A050088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 150, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A050088
 *  a(1) = 1
 *  For n>1:
 *      x = a(n‑1)/3   (integer division, i.e. floor)
 *      if x > 0 and x not in {a(1), … , a(n‑1)} then a(n) = x
 *      otherwise a(n) = 6*a(n‑1)
 *
 *  The implementation memoises all terms computed so far, which allows
 *  arbitrary order of calls.  The membership test uses a linear scan;
 *  for n≈10 000 this is well under the time limits for both C11 and
 *  C++17.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Mutable state holding all already computed terms */
    static seq_t *terms   = NULL;   /* array of terms */
    static size_t  len    = 0;       /* number of stored terms */
    static size_t  cap    = 0;       /* current capacity of terms array */

    if (n == 0)   /* safety: offset is 1, but return 0 for 0 index */
        return 0;

    /* Ensure we have computed up to term n */
    while (len < (size_t)n) {
        /* initialise first term */
        if (len == 0) {
            if (cap == 0) {
                cap = 16;
                terms = (seq_t *)malloc(cap * sizeof(seq_t));
            }
            terms[0] = 1;  /* a(1) = 1 */
            len = 1;
            continue;
        }

        seq_t prev = terms[len - 1];
        seq_t cand = prev / 3;   /* floor division for unsigned types */
        bool candValid = cand > 0;
        bool seen = false;

        if (candValid) {
            /* linear scan for existence */
            for (size_t i = 0; i < len; ++i) {
                if (terms[i] == cand) {
                    seen = true;
                    break;
                }
            }
        }

        seq_t next;
        if (candValid && !seen) {
            next = cand;
        } else {
            next = prev * 6;
        }

        /* extend storage if necessary */
        if (len == cap) {
            cap *= 2;
            terms = (seq_t *)realloc(terms, cap * sizeof(seq_t));
        }
        terms[len++] = next;
    }

    return terms[n - 1];
}
