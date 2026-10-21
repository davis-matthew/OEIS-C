/*
 * A226757  a(n) = number of distinct quadruples of squares modulo n.
 * https://oeis.org/A226757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = (number of distinct quadratic residues modulo n)^4
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 513, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) {
        return 0; /* Undefined for n == 0, but return 0 for safety */
    }

    /* Allocate a boolean array to mark which residues have been seen. */
    size_t sz = (size_t)n;
    bool *seen = (bool *)calloc(sz, sizeof(bool));
    if (!seen) {
        return 0; /* Allocation failure: treat as 0 */
    }

    seq_t count = 0;
    for (idx_t x = 0; x < n; ++x) {
        /* Compute x^2 mod n.  The intermediate 64‑bit product is safe
           because n <= 10 000, so x^2 <= 100 000 000. */
        idx_t sq = (x * x) % n;
        if (!seen[sq]) {
            seen[sq] = true;
            ++count;
        }
    }

    free(seen);

    /* Compute r^4 as (r^2)^2 using seq_t arithmetic. */
    seq_t r2 = count * count;
    return r2 * r2;
}
