/*
 * A147996  a(n) = 9^n-8^n-7^n-6^n-5^n-4^n-3^n-2^n-1.
 * https://oeis.org/A147996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 9^(n-1) - 8^(n-1) - 7^(n-1) - 6^(n-1) - 5^(n-1)
 *                          - 4^(n-1) - 3^(n-1) - 2^(n-1) - 1
 * (offset 1).  For n ≥ 1, `exp = n-1` is the exponent used in the
 * powers of 9 through 2.  The final constant term is 1, not 1^n.
 */

#include <stdint.h>

/* Binary exponentiation: computes base^exp in type seq_t. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1)
            result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)  /* not part of the sequence (offset 1), safe guard */
        return (seq_t)0;

    idx_t exp = n - 1;          /* exponent adjusted for offset 1 */

    seq_t result = pow_seq((seq_t)9, exp);
    result -= pow_seq((seq_t)8, exp);
    result -= pow_seq((seq_t)7, exp);
    result -= pow_seq((seq_t)6, exp);
    result -= pow_seq((seq_t)5, exp);
    result -= pow_seq((seq_t)4, exp);
    result -= pow_seq((seq_t)3, exp);
    result -= pow_seq((seq_t)2, exp);
    result -= (seq_t)1;         /* subtract the constant 1 */

    return result;
}
