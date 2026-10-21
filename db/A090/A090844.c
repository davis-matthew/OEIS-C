/*
 * A090844  Square array of numbers read by antidiagonals with
 *          T(n,k)=((k+3)(k+1)^n-2*0^n)/(k+1).
 * https://oeis.org/A090844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow, int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A090844.
   The square array is defined by
       T(0,k)     = 1,
       T(n,k)     = (k+3)*(k+1)^(n-1)   for n ≥ 1.
   The sequence a(n) is obtained by reading the array by antidiagonals
   (i.e., by all pairs (i , d-i) where d = i + (d-i) and the pairs
   are taken in increasing order of i).  For a given n we first find
   the diagonal d with d(d+1)/2 ≤ n < (d+1)(d+2)/2,
   then i = n - d(d+1)/2 and k = d-i.  The value is
   a(n) = 1                 if i = 0
   a(n) = (k+3)*(k+1)^(i-1) otherwise.  */

/* Dependencies */
#include <stdint.h>
#include "../utils/big_pow.h"        /* big_pow(seq_t base, seq_t exp) */
#include "../utils/int_sqrt.h"       /* int_sqrt(idx_t n) */

seq_t a(idx_t n)
{
    /* Special case n = 0 is handled quickly. */
    if (n == 0u) {
        return (seq_t)1;
    }

    /* Find diagonal d such that d(d+1)/2 ≤ n < (d+1)(d+2)/2. */
    idx_t eight_n = n * (idx_t)8;
    idx_t s = int_sqrt(eight_n + (idx_t)1);   /* floor( sqrt(1+8n) ) */
    idx_t d = (s - (idx_t)1) / (idx_t)2;

    /* Offsets to reach the beginning of diagonal d. */
    idx_t off = d * (d + (idx_t)1) / (idx_t)2;
    idx_t i = n - off;          /* position within the diagonal (0..d) */
    if (i == 0u) {
        return (seq_t)1;         /* first element on each diagonal is always 1 */
    }

    idx_t k = d - i;            /* corresponding column index */
    seq_t base = (seq_t)(k + (idx_t)1);       /* (k+1) */
    seq_t exp  = (seq_t)(i - (idx_t)1);       /* exponent i-1 */

    /* Compute (k+1)^(i-1) and multiply by (k+3). */
    seq_t pow_val = big_pow(base, exp);
    seq_t res = pow_val * ((seq_t)(k + (idx_t)3));
    return res;
}
