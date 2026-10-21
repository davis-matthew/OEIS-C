/*
 * A060551  a(n) is the number of nonsymmetric patterns (no reflective, nor
 *          rotational symmetry) which may be formed by an equilateral
 *          triangular arrangement of closely packed black and white cells
 *          satisfying the local matching rule of Pascal's triangle modulo 2,
 *          where n is the number of cells in each edge o...
 * https://oeis.org/A060551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n - 3*2^ceil(n/2) - 2^(floor(n/3)+(n mod 3) mod 2) +
 *            3*2^(floor((n+3)/6)+d(n)), d(n)=1 if n mod 6 = 1 else 0
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_exp.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t term1 = pow2_exp(n);

    idx_t ceil_n2 = (n + 1) / 2;          /* ceil(n/2) */
    seq_t term2 = pow2_exp(ceil_n2);
    term2 = term2 * 3;                   /* 3 * 2^ceil(n/2) */

    idx_t exp3 = n / 3 + ((n % 3) == 1 ? 1 : 0);
    seq_t term3 = pow2_exp(exp3);        /* 2^(floor(n/3)+(n%3==1?1:0)) */

    idx_t exp4 = (n + 3) / 6 + ((n % 6) == 1 ? 1 : 0);
    seq_t term4 = pow2_exp(exp4);
    term4 = term4 * 3;                   /* 3 * 2^((n+3)/6 + d(n)) */

    seq_t result = term1 + term4;
    result -= term2;
    result -= term3;
    return result;
}
