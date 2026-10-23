/*
 * A130208  Diagonalized matrix of A000203, sigma(n).
 * https://oeis.org/A130208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1815, u32 n <= 9328, u64 n <= 9328,
 *            u128 n <= 9328, big any n
 * Helpers:   floor_sqrt, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A130208 – Diagonalized matrix of sigma(n) (A000203)
 *
 *  a(n) =   sigma(k)   if  n = k*(k+1)/2  for some integer k ≥ 1
 *          0           otherwise
 *
 *  The function returns 0 for indices that are not triangular numbers
 *  and sigma(k) otherwise.  The computation uses the supplied
 *  integer square‑root routine `floor_sqrt` and the divisor‑sum
 *  routine `sigma_of`.  All arithmetic is performed in the
 *  sequence type `seq_t`, casting the index `n` to `seq_t` before
 *  multiplication, as required by the project specification.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0)                    /* offset is 1; n=0 not used */
        return 0;

    /*  discriminant of k^2 + k - 2n = 0  is 1 + 8n  */
    seq_t tn          = (seq_t)n;
    seq_t discriminant = tn * 8 + 1;

    seq_t s = floor_sqrt(discriminant);   /* integer part of sqrt(1+8n) */
    if (s * s != discriminant)           /* not a perfect square → not triangular */
        return 0;

    seq_t k = (s - 1) / 2;                /* solve k = (-1 + sqrt(1+8n))/2 */
    if (k == 0)                           /* guard against invalid k */
        return 0;

    /*  k is within the range of idx_t for the anticipated n values  */
    return sigma_of((idx_t)k);
}
