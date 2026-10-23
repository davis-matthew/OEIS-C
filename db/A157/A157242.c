/*
 * A157242  Number of ways to write the n-th positive odd integer in the form
 *          p+2^x+11*2^y with p a prime congruent to 5 mod 6 and x,y positive
 *          integers.
 * https://oeis.org/A157242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = # {(p,x,y) : p + 2^x + 11·2^y = 2·n – 1, * p prime ≡ 5
 *            (mod 6), x>0, y>0}
 * Range:     u8 n <= 9442, u16 n <= 9442, u32 n <= 9442, u64 n <= 9442,
 *            u128 n <= 9442, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function directly enumerates all admissible pairs (x,y).   */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 2·n–1 must be odd; smallest possible sum is 5 + 2 + 22 = 29
       which corresponds to n = 15.  For smaller n the count is 0.  */
    if (n < 15)
        return 0;

    idx_t target = (idx_t)n * 2ULL - 1ULL;   /* 2·n–1 */
    seq_t   cnt   = 0;

    /* iterate over positive y   ( 11·2^y part ) */
    for (idx_t y = 1; ; ++y) {
        idx_t powy = (idx_t)11ULL << y;      /* 11 * 2^y */
        if (powy > target - 5ULL)            /* no room for a prime ≥5 */
            break;

        /* iterate over positive x   ( 2^x part ) */
        for (idx_t x = 1; ; ++x) {
            idx_t powx = (idx_t)1ULL << x;   /* 2^x */
            if (powx > target - 5ULL - powy)
                break;

            idx_t p = target - powx - powy; /* candidate prime */
            if ((p % 6ULL) == 5ULL && is_prime(p))
                ++cnt;
        }
    }
    return cnt;
}
