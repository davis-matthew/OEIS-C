/*
 * A065917  Boundaries of primorial intervals [1,3]; [3,9],[9,15]; [15,45],
 *          etc.
 * https://oeis.org/A065917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=3.
   For n≥2, let q(j) be the j‑th primorial (product of the first j primes).
   If   q(j) < a(n+1) ≤ q(j+1)/2,  then   a(n+1) = a(n) + q(j). */

/* NOTE:  The implementation uses an incremental method:
   the current primorial q(j) is kept in  a  variable and the
   next primorial q(j+1) is computed on the fly.
   The helper below finds the next prime after a given prime.   */

#include "../utils/is_prime_small.h"
#include <stdint.h>   /* for uint64_t */

/* return the smallest prime strictly greater than n (n is a prime) */
static uint64_t next_prime_gt(uint64_t n)
{
    uint64_t cand = n + 1;
    if (cand <= 2) return 2;
    if ((cand & 1ULL) == 0) cand += 1;   /* make odd */
    while (!is_prime_small(cand))
        cand += 2;
    return cand;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1) = 1 */

    /* current primorial q(j)  and the next one q(j+1)   */
    seq_t qj          = (seq_t)2;          /* q(1) = 2           */
    uint64_t next_p   = 3;                 /* prime for q(j+1)    */
    seq_t qj1         = qj * (seq_t)next_p;/* q(2) = 6           */

    seq_t val = (seq_t)1;                 /* a(1)                */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* If the next step would exceed the upper bound,
           advance to the next primorial.               */
        while (val + qj > qj1 / (seq_t)2)
        {
            qj = qj1;                          /* q(j) ← q(j+1)      */
            next_p = next_prime_gt(next_p);    /* next prime           */
            qj1 = qj * (seq_t)next_p;          /* q(j+1) = q(j)*prime */
        }
        val += qj;                            /* a(n+1) = a(n)+q(j)   */
    }

    return val;
}
