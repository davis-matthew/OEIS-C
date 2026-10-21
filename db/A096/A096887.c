/*
 * A096887  Initial values for f(x)=phi(sigma(x)) such that iteration of f
 *          ends in cycle of length=2.
 * https://oeis.org/A096887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 3969, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = n-th integer x such that the orbit of
 *  f(x) = phi(sigma(x)) under iteration eventually ends in a 2‑cycle.
 *  Computed by brute‑force search with memoisation. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n); */
#include "../utils/phi.h"        /* seq_t phi(idx_t m); */

/* Test whether the orbit of start eventually hits a 2‑cycle.
 * We keep a small history of seen values (up to 4096 steps) to
 * detect a cycle.  If a cycle of length 2 is found, return true;
 * otherwise return false (or if the search limit is exceeded). */
static bool ends_in_two_cycle(uint64_t start)
{
    /* history of visited values (up to 4096 steps) */
    uint64_t seen[4096];
    int seen_idx[4096];
    int seen_len = 0;
    int step = 0;
    uint64_t x = start;

    while (1) {
        /* look for a previous occurrence */
        for (int i = 0; i < seen_len; ++i) {
            if (seen[i] == x) {
                int cycle_len = step - seen_idx[i];
                return cycle_len == 2;
            }
        }

        if (step >= (int)(sizeof(seen) / sizeof(seen[0])))
            return false;          /* safety limit exceeded */

        /* record current value */
        seen[seen_len] = x;
        seen_idx[seen_len] = step;
        ++seen_len;

        /* compute next value: f(x) = phi(sigma(x)) */
        seq_t s = sigma_of((idx_t)x);
        seq_t f = phi((idx_t)s);
        x = (uint64_t)f;
        ++step;
    }
}

/* --------------------------------------------------------------------- */
/*  Memoised lookup of the sequence                                            */
/* --------------------------------------------------------------------- */
static seq_t *found     = NULL;   /* array of found terms      */
static size_t found_cap = 0;      /* allocated capacity        */
static size_t found_size = 0;     /* number of terms stored    */
static uint64_t last_checked = 0; /* last integer examined     */

seq_t a(idx_t n)
{
    if (n == 0)                     /* offset 1; undefined for n==0 */
        return 0;

    /* Extend the cache until we have at least n terms. */
    while (found_size < (size_t)n) {
        ++last_checked;                         /* next candidate number   */

        if (ends_in_two_cycle(last_checked)) {   /* satisfies condition?   */
            /* Grow storage if needed */
            if (found_size == found_cap) {
                size_t new_cap = (found_cap == 0 ? 8 : found_cap * 2);
                seq_t *tmp = (seq_t *)realloc(found,
                                               new_cap * sizeof(seq_t));
                if (!tmp) {                      /* allocation failed */
                    free(found);
                    found = NULL;
                    found_cap = found_size = 0;
                    return 0;
                }
                found = tmp;
                found_cap = new_cap;
            }
            found[found_size++] = (seq_t)last_checked;
        }
    }

    return found[n - 1];
}
