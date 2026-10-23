/*
 * A015432  Gaussian binomial coefficient [ n,12 ] for q=-9.
 * https://oeis.org/A015432
 *
 * Offset:    12    (a(12) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..12} ((-9)^(n-i+1)-1)/((-9)^i-1)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n)
{
    /* The Gaussian binomial coefficient [n,12] for q = -9.
       All terms are positive; we ignore the overall sign which
       cancels out by symmetry.  The factorization
             ((-9)^k - 1) = { 9^k - 1   (k even)
                               -(9^k + 1) (k odd) }
       leads to the equivalent magnitude
             f(k) = 9^k - 1          (k even)
             f(k) = 9^k + 1          (k odd).
       Hence
             a(n) = Π_{i=1}^{12} f(n-i+1) / f(i) .
       We compute the 12 numerators and 12 denominators,
       cancel common factors while they are small, and finally
       multiply the remaining numerators.  This avoids
       intermediate overflow for any seq_t type. */

    if (n < 12)      /* behaviour below the supplied offset is irrelevant */
        return 0;

    const seq_t nine = (seq_t)9;
    /* -----------------------------------------
       Compute 9^n once and produce the 12
       successive powers 9^n, 9^{n-1}, …, 9^{n-11}.
       ----------------------------------------- */
    seq_t pow9_n = big_pow(nine, n);          /* 9^n */
    seq_t nums[12];                          /* f(n-i+1) values */
    seq_t cur_pow = pow9_n;                  /* current 9^k value */

    for (idx_t i = 1; i <= 12; ++i) {
        idx_t exp = n - i + 1;                /* exponent for this term */
        seq_t val = cur_pow;                 /* 9^exp */
        if ((exp & 1) == 0)                  /* even exponent */
            nums[i - 1] = val - 1;           /* f(exp) = 9^exp - 1 */
        else
            nums[i - 1] = val + 1;           /* f(exp) = 9^exp + 1 */
        cur_pow /= nine;                     /* next power: 9^{exp-1} */
    }

    /* -----------------------------------------
       Compute f(i) for i = 1…12 using small powers.
       ----------------------------------------- */
    seq_t dens[12];                          /* denominator values */
    seq_t pow_small = 1;                     /* 9^{i-1} */
    for (idx_t i = 1; i <= 12; ++i) {
        pow_small *= nine;                   /* 9^i */
        if ((i & 1) == 0)
            dens[i - 1] = pow_small - 1;    /* even i : 9^i - 1 */
        else
            dens[i - 1] = pow_small + 1;    /* odd  i : 9^i + 1 */
    }

    /* -----------------------------------------
       Cancel common factors between numerators
       and denominators.  The double loop is
       sufficient because we have only 12 terms.
       ----------------------------------------- */
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            if (dens[j] == 1) continue;
            seq_t g = seq_gcd(nums[i], dens[j]);
            if (g > 1) {
                nums[i] /= g;
                dens[j] /= g;
            }
        }
    }

    /* -----------------------------------------
       Multiply the remaining numerators to get
       the final value.  All denominators should
       now be 1; if any exception remains it can
       be divided out safely (it must divide exact).
       ----------------------------------------- */
    seq_t result = 1;
    for (int i = 0; i < 12; ++i)
        result *= nums[i];

    for (int i = 0; i < 12; ++i)
        if (dens[i] != 1)
            result /= dens[i];

    return result;
}
