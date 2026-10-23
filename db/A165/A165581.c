/*
 * A165581  Legendre symbol (n,524287).
 * https://oeis.org/A165581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = LegendreSymbol(n, 524287)
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 2, i64 n <= 9794, i128 n <= 9794,
 *            big any n
 * Helpers:   legendre_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = n^262143 (mod 524287); if the residue is 524286, return -1; */
/*          if n is divisible by 524287, return 0; */

#include <stdbool.h>
#include "../utils/legendre_pow.h"

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)524287;          /* 2^19 - 1, a Mersenne prime       */
    const idx_t EXP = 262143;                /* (MOD-1)/2                         */

    /* Reduce n modulo MOD to avoid large intermediate values */
    seq_t nmod = (seq_t)(n % (idx_t)MOD);

    /* n is multiple of the modulus → value 0 */
    if (nmod == 0) return (seq_t)0;

    /* Compute the Legendre symbol using Euler's criterion */
    seq_t r = legendre_pow(nmod, EXP, MOD);

    if (r == MOD - 1) return (seq_t)-1;  /* p-1 represents -1 in Legendre symbol */
    return (seq_t)1;                      /* otherwise the residue is 1           */
}
