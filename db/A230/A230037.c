/*
 * A230037  Number of ways to write n = x + y + z (0 < x <= y <= z) such that
 *          the four pairs {6*x-1, 6*x+1}, {6*y-1, 6*y+1}, {6*z-1, 6*z+1} and
 *          {6*x*y-1, 6*x*y+1} are twin prime pairs.
 * https://oeis.org/A230037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5844, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A230037
 *  Number of ways to write n = x + y + z (0 < x ≤ y ≤ z)
 *  such that the four pairs {6x-1,6x+1}, {6y-1,6y+1},
 *  {6z-1,6z+1} and {6xy-1,6xy+1} are twin prime pairs.
 *
 *  The implementation pre‑computes which indices k (1 ≤ k ≤ n)
 *  form a twin prime pair (i.e. 6k-1 and 6k+1 are both prime)
 *  by testing them with is_prime.  Then it enumerates all
 *  admissible triples (x,y,z) with x ≤ y and z = n-x-y
 *  checking the twin prime condition for z and for the product
 *  6xy.  The count is returned as a seq_t value.
 */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    if (n < 3)                     /* no solutions for n = 1,2 */
        return (seq_t)0;

    /* mark indices that themselves form a twin prime pair */
    bool *twin = (bool *)malloc((n + 1) * sizeof(bool));
    if (!twin)                     /* allocation failure – unlikely */
        return (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        bool p1 = is_prime(6 * k - 1);
        bool p2 = is_prime(6 * k + 1);
        twin[k] = p1 && p2;
    }

    seq_t count = 0;

    for (idx_t x = 1; x <= n; ++x) {
        if (!twin[x])
            continue;                /* x must be a twin index */

        /* y ranges from x to (n - x)/2 so that z = n - x - y ≥ y */
        idx_t y_max = (n - x) / 2;
        for (idx_t y = x; y <= y_max; ++y) {
            if (!twin[y])
                continue;            /* y must be a twin index */

            idx_t z = n - x - y;
            if (z < y || z > n)      /* enforce x ≤ y ≤ z */
                continue;
            if (!twin[z])
                continue;            /* z must be a twin index */

            /* check the twin condition for the product 6*x*y */
            idx_t prod = 6ULL * x * y;
            if (!is_prime(prod - 1) || !is_prime(prod + 1))
                continue;

            count += 1;              /* one valid triple found */
        }
    }

    free(twin);
    return count;
}
