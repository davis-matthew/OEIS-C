/*
 * A205555  Least positive integer j such that n divides k^(k-1)-j^(j-1),
 *          where k (as in A205554) is the least positive integer for which
 *          there is such a j.
 * https://oeis.org/A205555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A205555:  a(n) = least j<k such that
 * n | k^(k-1) - j^(j-1), where k is the smallest integer >1 with this property. 
 * The algorithm iterates m = 1,2,…, computing f(m) = m^(m-1) mod n.
 *  f(1) is stored as the first j for residue f(1).
 *  For subsequent m (candidate k) we test if f(m) was seen before.
 *  The first duplicate occurrence gives the desired k and the stored j.  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not used, but guard */

    /* array mapping residues [0, n-1] to the first index (j) that produced them */
    idx_t *first = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    if (!first) return 0;               /* allocation failure, return 0 as safe fallback */
    for (size_t i = 0; i < (size_t)n; ++i) first[i] = 0;

    for (idx_t m = 1; ; ++m) {
        /* compute f(m) = m^(m-1) mod n  */
        idx_t base = (n == 0) ? 0 : (m % n);             /* against division by zero */
        idx_t exp  = m - 1;
        idx_t res  = 1 % n;                               /* 0 when n==1, else 1 */

        /* fast exponentiation */
        while (exp > 0) {
            if (exp & 1) res = (res * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }

        if (first[res] != 0) {
            /* found k = m with prior j = first[res] */
            seq_t j_res = (seq_t)first[res];
            free(first);
            return j_res;
        } else {
            /* store this m as the first index producing this residue */
            first[res] = m;
        }
    }
}
