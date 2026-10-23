/*
 * A049457  Least positive integer k such that the number having periodic
 *          continued fraction [ 1,m,1,m,1,m,... ] is of form (a+b*sqrt(k))/c,
 *          where a,b,c are positive integers.
 * https://oeis.org/A049457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = squarefree part of n*(n+4)
 * Range:     u8 n <= 14, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The minimal k of the form m(m+4)/w^2 with m = n, w ∈ ℕ is obtained
   by dividing n*(n+4) by its largest square divisor.  This is the
   squarefree kernel of n*(n+4). */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    /* Compute product = n*(n+4) in seq_t */
    seq_t product = (seq_t)n * ((seq_t)n + 4);

    seq_t result = 1;          /* squarefree part will accumulate here */
    seq_t temp = product;      /* working copy for factorisation */

    /* Trial division: iterate all possible prime candidates p.
       p may be more than squarefree, but the loop stops when p^2 > temp. */
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= temp; ++p) {
        if (temp % (seq_t)p == 0) {
            int exp = 0;
            /* Count the exponent of p in temp */
            while (temp % (seq_t)p == 0) {
                temp /= (seq_t)p;
                exp++;
            }
            /* Keep one factor of p only if its exponent is odd */
            if (exp & 1)
                result *= (seq_t)p;
        }
    }

    /* If remaining temp is > 1, it is a prime factor with exponent 1,
       hence it must be included in the squarefree part. */
    if (temp > 1)
        result *= temp;

    return result;
}
