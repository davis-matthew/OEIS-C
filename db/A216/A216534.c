/*
 * A216534  Number of cycles in all partial functions on {1,2,...,n}.
 * https://oeis.org/A216534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{l=1..n} n! / ( l · (n–l)! ) · (n+1)^{ n–l }
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* which can be computed by the recurrence
       t_1 = n · (n+1)^{ n-1 }
       t_{l+1} = t_l · l · (n–l) / ((l+1) · (n+1))
   and a(n) = Σ_{l=1..n} t_l
   (valid for all n ≥ 0, with a(0)=0). */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* base case */

    /* compute (n+1)^(n-1) once */
    seq_t base  = (seq_t)n + 1;
    seq_t exp   = n - 1;           /* idx_t converted to seq_t */
    seq_t power = big_pow(base, exp);

    /* t_1 = n · (n+1)^{n-1} */
    seq_t cur = power * (seq_t)n;
    seq_t sum = cur;

    /* iterate l = 1 .. n-1 */
    for (idx_t l = 1; l < n; ++l) {
        seq_t num = cur * (seq_t)l;
        num = num * (seq_t)(n - l);

        seq_t denom = (seq_t)(l + 1) * (seq_t)(n + 1);
        seq_t next = num / denom;

        cur = next;
        sum += cur;
    }

    return sum;
}
