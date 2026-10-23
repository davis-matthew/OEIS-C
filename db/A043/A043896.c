/*
 * A043896  Numbers k such that 0 and 1 occur juxtaposed in the base-6
 *          representation of k but not of k+1.
 * https://oeis.org/A043896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A043896 – numbers k such that 0 and 1 occur juxtaposed (in
 *            either order) in the base‑6 representation of k, but not in k+1
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper: check if a base‑6 representation of x contains
   adjacent digits 0 and 1 in either order. */
static bool has_adjacent_01(seq_t x)
{
    unsigned prev = 7;                /* 7 is not a legal base‑6 digit */
    while (x > 0) {
        unsigned d = (unsigned)(x % 6);
        if (prev != 7 &&
            ((prev == 0 && d == 1) || (prev == 1 && d == 0)))
            return true;
        prev = d;
        x /= 6;
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* static storage for memoization */
    static seq_t *cache = NULL;      /* array of found terms */
    static idx_t     count = 0;      /* number of terms stored          */
    static seq_t     last_k = 0;     /* last checked integer            */
    static size_t    cap   = 0;      /* current capacity of cache       */

    /* produce terms until we have at least n of them */
    while (count < n) {
        last_k++;                    /* examine the next integer */

        if (has_adjacent_01(last_k) &&
            !has_adjacent_01(last_k + (seq_t)1)) {

            /* grow cache if necessary */
            if (count >= cap) {
                size_t newcap = cap == 0 ? 16 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              newcap * sizeof(seq_t));
                if (tmp == NULL) {            /* unrecoverable */
                    abort();
                }
                cache = tmp;
                cap   = newcap;
            }

            cache[count++] = last_k;  /* store the found term */
        }
    }

    /* now cache[n-1] holds the nth term (offset 1) */
    return cache[n - 1];
}
