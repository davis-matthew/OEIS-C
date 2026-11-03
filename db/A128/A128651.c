/*
 * A128651  In the sequence of natural numbers, replace the nonsquarefree
 *          numbers with their minimal factorization into squarefree numbers.
 * https://oeis.org/A128651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f(n,1,2,2,1) with f(n,m,x,y,z) = * if n=m then z * else
 *            if rad(y)=y then f(n,m+1,x+1,x+1,y) * else
 *            f(n,m+1,x,y/rad(y),rad(y)) * where rad = squarefree kernel
 *            (A007947)
 * Range:     u8 n <= 432, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 9868 (2026-11-01)
 * Helpers:   radical
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/radical.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* safety guard; the sequence is defined for n ≥ 1 */
    idx_t m = 1;
    idx_t x = 2;
    idx_t y = 2;
    seq_t z = 1;

    while (m < n) {
        seq_t r_seq = radical(y);          /* squarefree kernel of y */
        idx_t r = (idx_t)r_seq;

        if (r == y) {                  /* y is squarefree */
            z = (seq_t)y;               /* store previous y */
            ++x;                        /* increment x */
            y = x;                      /* update y to new x */
        } else {                        /* y is not squarefree */
            z = r_seq;                  /* squarefree kernel */
            y /= r;                     /* reduce y */
            /* x remains unchanged */
        }
        ++m;
    }
    return z;
}
