/*
 * A093230  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 30.
 * https://oeis.org/A093230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (10^(n-1) - 1) * 10^n
 *
 * This works for all n ≥ 1.
 * For n = 1 the result is 0.
 * The implementation uses only integer arithmetic in seq_t
 * and a fast exponentiation routine for 10^k.
 */

#include <stdint.h>   /* for uint64_t, but seq_t and idx_t are defined elsewhere */

static seq_t pow10(idx_t e)
{
    seq_t result = 1;      /* 10^0 = 1 */
    seq_t base   = 10;     /* 10^1 = 10 */
    while (e > 0)
    {
        if (e & 1)
            result = result * base;
        base   = base * base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)          /* a(1) = 0 */
        return 0;
    /* For n ≥ 2 */
    seq_t p = pow10(n - 1);  /* 10^(n-1) */
    seq_t q = pow10(n);      /* 10^n     */
    return (p - 1) * q;
}
