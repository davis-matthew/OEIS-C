/*
 * A204935  The number j! such that n divides k!-j!>0, where k is the least
 *          positive integer for which such a j exists.
 * https://oeis.org/A204935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 64, u32 n <= 64, u64 n <= 192,
 *            u128 n <= 382, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * a(n) = the factorial j! such that n | k! – j! > 0, with k the
 * smallest positive integer for which such a j exists.
 * The algorithm finds the first repeated residue of factorials modulo n;
 * the earlier index j of that residue gives a(n) = j!.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n >= 1 is assumed; guard against zero for robustness */
    if (n == 0) return 0;

    size_t m = (size_t)n;                      /* table size */
    bool *seen  = (bool *)calloc(m, sizeof(bool));
    idx_t *first = (idx_t *)calloc(m, sizeof(idx_t));
    if (!seen || !first) {
        free(seen);
        free(first);
        return 0;
    }

    /* initial residue 1! mod n */
    idx_t r = (idx_t)(1 % n);
    seen[r]   = true;
    first[r]  = 1;

    idx_t k;
    for (k = 2; ; ++k) {
        r = (idx_t)((r * k) % n);              /* (k! mod n) */

        if (seen[r]) {                         /* repeated residue */
            idx_t j = first[r];

            /* compute j! exactly in seq_t */
            seq_t val = 1;
            for (idx_t t = 2; t <= j; ++t)
                val *= (seq_t)t;

            free(seen);
            free(first);
            return val;
        }

        seen[r]  = true;
        first[r] = k;
    }
}
