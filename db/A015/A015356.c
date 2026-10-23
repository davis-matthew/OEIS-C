/*
 * A015356  Gaussian binomial coefficient [ n,8 ] for q=-2.
 * https://oeis.org/A015356
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..8} ((-2)^(n-i+1)-1)/((-2)^i-1)
 * Range:     u8 none, u16 n <= 8, u32 n <= 11, u64 n <= 16, u128 n <= 24,
 *            big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_gcd.h"

/* Compute 2^exp as a seq_t.  For seq_t that fits in 64 bits we use
   an intermediate uint128_t when the shift would overflow 64 bits. */
static inline seq_t pow2_shift(idx_t exp)
{
    /* If seq_t is no larger than uint64_t, be careful about
       shifting 64 bits or more.  Use __int128 for the shift.   */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        if (exp >= 64) {
            unsigned __int128 pw = ((unsigned __int128)1) << exp;
            return (seq_t)pw;
        } else {
            return (seq_t)((unsigned long long)1 << exp);
        }
    }
    /* For arbitrary size seq_t the shift operator works (library
       overloads it for big integer classes). */
    return ((seq_t)1) << exp;
}

seq_t a(idx_t n)
{
    if (n < 8) return 0;                     /* offset is 8 */

    seq_t result = 1;

    for (idx_t i = 1; i <= 8; ++i) {
        idx_t exp_num = n - i + 1;           /* exponent for numerator   */
        seq_t pw_num   = pow2_shift(exp_num);
        seq_t a_num = (exp_num & 1) ? (pw_num + 1) : (pw_num - 1); /* |(-2)^m-1| */

        seq_t pw_den   = pow2_shift(i);
        seq_t a_den = (i & 1) ? (pw_den + 1) : (pw_den - 1);       /* |(-2)^i-1| */

        /* cancel common factors between numerator and denominator   */
        seq_t g1 = seq_gcd(a_num, a_den);
        a_num /= g1;
        a_den /= g1;

        /* cancel common factors between current result and denom   */
        seq_t g2 = seq_gcd(result, a_den);
        result /= g2;
        a_den /= g2;          /* a_den should now be 1              */

        result *= a_num;      /* multiply by the reduced numerator   */
    }
    return result;
}
