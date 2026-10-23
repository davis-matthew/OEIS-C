/*
 * A035193  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          11.
 * https://oeis.org/A035193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(11,d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   We use the multiplicative property of the divisor sum of an
   arithmetic function.  For a prime power p^e:

      if p == 11            => a(p^e) = 1
      else
          s = Kronecker(11,p)   (Legendre symbol)
          if s ==  1  => a(p^e) = e+1
          if s == -1  => a(p^e) = (e even ? 1 : 0)
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"    /* factor_u64(n, primes, exps, &cnt)   */
#include "../utils/jacobi_symbol.h"/* jacobi_symbol(a, n) --> (-1,0,1)      */

static seq_t factor_value(uint64_t p, uint64_t e)
{
    if (p == 11U)               /* special prime */
        return (seq_t)1U;

    int s;
    if (p == 2U)
        /* Kronecker(11,2) = (-1)^( (11^2-1)/8 ) = -1 */
        s = -1;
    else
        s = jacobi_symbol(11U, p);   /* Legendre symbol since p is odd */

    if (s == 1)
        return (seq_t)(e + 1U);
    else /* s == -1 */
        return ((e & 1U) == 0U) ? (seq_t)1U : (seq_t)0U;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                          /* not expected for this sequence */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result *= factor_value(primes[i], exps[i]);

    return result;
}
