/*
 * A185251  a(n) = Sum_{k=0..floor(n/2)} k*binomial(n,k).
 * https://oeis.org/A185251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} k*binomial(n,k)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases.  For n = 0 or 1 the sum is 0. */
    if (n < 2)
        return (seq_t)0;

    /* term = n * 2^{n-2}  (fits in seq_t since the division below
       ensures the result is integral). */
    seq_t term = pow2(n - 2);          /* 2^(n-2) */
    term = term * (seq_t)n;            /* n * 2^(n-2) */

    /* Even n:  a(n) = n * 2^{n-2}. */
    if ((n & 1U) == 0U)
        return term;

    /* Odd n:  a(n) = n*2^{n-2} - ((n+1)*binomial(n,(n-1)/2))/4 */
    idx_t k = (n - 1) / 2;             /* (n-1)/2 */
    seq_t bin = binomial(n, k);        /* binomial(n, k) */
    seq_t tmp = bin * (seq_t)(n + 1);  /* (n+1) * C(n,k)  */
    tmp = tmp / (seq_t)4;              /* division is exact */

    return term - tmp;
}
