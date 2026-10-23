/*
 * A134937  Squares that are the sum of a nonzero pentagonal number and a
 *          nonzero square in at least one way.
 * https://oeis.org/A134937
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A134937 – Squares that are the sum of a nonzero pentagonal
 *            number * and a nonzero square in at least one way. The function
 *            returns the * n‑th such square (OEIS offset 1). An exhaustive
 *            search over squares is * performed; for each square r² we test
 *            all pentagonal numbers P(k)=k(3k‑1)/2 * less than...
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

static seq_t pentagonal(idx_t k)
{
    /* P(k) = k(3k-1)/2  */
    seq_t kk = (seq_t)k;
    return kk * ((kk * 3) - 1) / 2;
}

seq_t a(idx_t n)
{
    idx_t found = 0;          /* number of squares already found */
    idx_t r = 1;              /* current square root being tested */

    for (;; ++r)
    {
        seq_t r2 = ((seq_t)r) * r;          /* r² */

        /* iterate k while P(k) < r² */
        for (idx_t k = 1; ; ++k)
        {
            seq_t p = pentagonal(k);
            if (p >= r2)                    /* P(k) too large: stop */
                break;

            seq_t diff = r2 - p;            /* candidate square difference */
            if (diff == 0)                  /* would give m = 0: ignore */
                continue;

            seq_t s = floor_sqrt(diff);     /* floor(sqrt(diff)) */
            if (s * s == diff)             /* perfect square? */
            {
                ++found;
                if (found == n)             /* n‑th term found */
                    return r2;
                break;                     /* move to next r, no need more k */
            }
        }
        /* continue with next r if not found yet */
    }
}
