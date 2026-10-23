/*
 * A185439  Emirp gaps: Differences between consecutive emirps.
 * https://oeis.org/A185439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A006567(n+1) – A006567(n)
 * Range:     u8 n <= 19, u16 n <= 4277, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  (i.e. the difference between consecutive emirps) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/rev.h"

static inline void grow_arr(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0) ? 4 : (*cap << 1);
        while (newcap < need) newcap <<= 1;
        seq_t *nptr = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
        if (!nptr) exit(1);              /* allocation failed */
        *arr   = nptr;
        *cap   = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined for index 0 */

    /* static data shared across all calls */
    static idx_t curr     = 2;           /* next integer to test */
    static bool   first   = true;        /* first emirp not yet found */
    static idx_t prev_em  = 0;           /* previous emirp value */

    static seq_t *diffs     = NULL;     /* gaps array */
    static size_t diff_cnt  = 0;
    static size_t diff_cap  = 0;

    /* generate emirps until we have produced at least n+1 of them,
       so that the gap a(n) exists */
    while (diff_cnt < n) {
        /* skip even numbers > 2 */
        if (curr > 2 && (curr & 1) == 0) {
            curr++;
            continue;
        }

        if (is_prime(curr)) {
            idx_t r = rev(curr);
            if (r != curr && is_prime(r)) {
                if (!first) {
                    /* gap between this emirp and the previous one */
                    seq_t gap = (seq_t)(curr - prev_em);
                    grow_arr(&diffs, &diff_cap, diff_cnt + 1);
                    diffs[diff_cnt++] = gap;
                } else {
                    /* first emirp found */
                    first = false;
                }
                prev_em = curr;
            }
        }
        curr++;
    }

    return diffs[n - 1];
}
