/*
 * A096364  Number of ways to generate a Coxeter element of the reflection
 *          group of the root system B_n with certain restrictions on
 *          generators: (3n-4)*(n-2)^(n-2) - (n-1)^(n-1).
 * https://oeis.org/A096364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3*n-4)*(n-2)^(n-2) - (n-1)^(n-1) for n >= 3. a(1) = 0,
 *            a(2) = 1, a(3) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    while (exp != 0) {
        if (exp & (idx_t)1)
            result *= b;
        exp >>= 1;
        if (exp != 0)
            b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;
    if (n == (idx_t)2)
        return (seq_t)1;
    if (n == (idx_t)3)
        return (seq_t)1;

    /* term1 = (3*n-4) * (n-2)^(n-2) */
    seq_t term1_pow = pow_seq((seq_t)(n - (idx_t)2), n - (idx_t)2);
    seq_t term1 = ((seq_t)n * (seq_t)3 - (seq_t)4) * term1_pow;

    /* term2 = (n-1)^(n-1) */
    seq_t term2 = pow_seq((seq_t)(n - (idx_t)1), n - (idx_t)1);

    return term1 - term2;
}
