/*
 * A230040  Number of ways to write n = x + y + z with y <= z such that all
 *          the five numbers 6*x-1, 6*y-1, 6*z-1, 6*x*y-1 and 6*x*z-1 are
 *          Sophie Germain primes.
 * https://oeis.org/A230040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2157, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A230040 – Count the ordered triples (x,y,z) with x + y + z = n,
 * y <= z such that 6*x−1, 6*y−1, 6*z−1, 6*x*y−1 and 6*x*z−1 are all
 * Sophie Germain primes.  The implementation enumerates all
 * admissible values.
 */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"   /* provides bool isPrimeGeneric(seq_t) */

/* Test whether a number is a Sophie Germain prime. */
static inline bool is_sg_prime(seq_t p)
{
    if (!isPrimeGeneric(p))
        return false;
    seq_t q = (seq_t)2 * p + 1;
    return isPrimeGeneric(q);
}

seq_t a(idx_t n)
{
    /* No decomposition possible for n = 1 or 2. */
    if (n < 3)
        return 0;

    /* Collect x where 6*x−1 is a Sophie Germain prime. */
    size_t need = (size_t)n;                     /* at most n−2 such values */
    idx_t *good = (idx_t *)malloc(need * sizeof(idx_t));
    if (!good)                                   /* allocation failure */
        return 0;

    size_t goodlen = 0;
    for (idx_t x = 1; x <= n - 2; ++x) {
        seq_t px = (seq_t)6 * x - 1;
        if (is_sg_prime(px))
            good[goodlen++] = x;
    }

    seq_t ans = 0;

    /* Enumerate all admissible (x, y, z).  y <= z ensures each
       decomposition is counted once. */
    for (size_t ix = 0; ix < goodlen; ++ix) {
        idx_t x = good[ix];
        for (size_t iy = 0; iy < goodlen; ++iy) {
            idx_t y = good[iy];
            idx_t z = n - x - y;
            if (z < y)            /* enforce y <= z */
                continue;
            if (z < 1)
                continue;

            seq_t pz  = (seq_t)6 * z - 1;
            if (!is_sg_prime(pz))
                continue;

            seq_t pxy = (seq_t)6 * x * y - 1;
            if (!is_sg_prime(pxy))
                continue;

            seq_t pxz = (seq_t)6 * x * z - 1;
            if (!is_sg_prime(pxz))
                continue;

            ++ans;
        }
    }

    free(good);
    return ans;
}
