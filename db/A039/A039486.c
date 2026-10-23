/*
 * A039486  Numbers whose base-11 representation has the same nonzero number
 *          of 6's and 10's.
 * https://oeis.org/A039486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n-th positive integer whose base‑11 representation
 *            contains the same nonzero number of digits 6 and 10
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   has_equal_nonzero_6_and_10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compile with:  -std=c11   (C)  or  -std=c++17   (C++)  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/has_equal_nonzero_6_and_10.h"

/*  External linkage – the requested function */
seq_t a(idx_t n)
{
    static seq_t *seq = NULL;      /* cache of found terms */
    static idx_t len = 0;          /* number of terms cached */
    static size_t cap = 0;         /* capacity of the cache  */
    static seq_t next_candidate = 1;  /* next number to test */

    if (n == 0)
        return 0;   /* undefined for n < 1 – return 0 as a placeholder */

    if (len < n) {                  /* need to generate more terms */
        while (len < n) {
            /* Find the next valid number */
            while (!has_equal_nonzero_6_and_10(next_candidate))
                ++next_candidate;

            /* Ensure cache capacity */
            if (len >= cap) {
                size_t newcap = (cap == 0) ? 4 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
                if (!tmp)
                    exit(1);               /* allocation failed */
                seq = tmp;
                cap = newcap;
            }

            /* Store the found term */
            seq[len++] = next_candidate;
            ++next_candidate;          /* advance to next candidate */
        }
    }
    return seq[n - 1];
}
