/*
 * A079847  Smallest multiple of n in which the string of digits of n occurs
 *          after (n-1) most significant digits.
 * https://oeis.org/A079847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079847:  smallest multiple of n in which the decimal string of n
   appears starting at position n (i.e., after n‑1 most significant digits).
   Formula (from OEIS):
       let d = number_of_digits(n)
       let n' = n / gcd(n, 10^d)
       let P  = 10^(n-2)
       a(n) = (P + ((-P) mod n')) * 10^d + n
   For integers n > 1, this is equivalent to
       a(n) = ceil(P / n') * n' * 10^d + n
   (the expression inside is the smallest multiple of n' that is >= P).
   For n = 1, a(1) = 1 by definition. */

#include <stdint.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t, seq_t) */

static seq_t pow10_seq(idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r *= (seq_t)10;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;               /* a(1) = 1 */

    /* 1. compute number of decimal digits d of n */
    idx_t d = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        ++d;
        tmp /= 10;
    }

    /* 2. 10^d  (small) */
    seq_t pow10d = (seq_t)1;
    for (idx_t i = 0; i < d; ++i)
        pow10d *= (seq_t)10;

    /* 3. n' = n / gcd(n, 10^d) */
    seq_t n_big = (seq_t)n;
    seq_t g = gcd(n_big, pow10d);
    seq_t nprime = n_big / g;          /* nprime > 0 */

    /* 4. P = 10^(n-2) */
    seq_t P = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        P *= (seq_t)10;                 /* n>=2 so loop may be zero length */

    /* 5. remainder of P modulo nprime */
    seq_t rem = P % nprime;

    /* 6. ceil division of P by nprime */
    seq_t quot = P / nprime;          /* integer division */
    seq_t ceilMult = quot + (rem == 0 ? (seq_t)0 : (seq_t)1);

    /* 7. Construct the result */
    seq_t result = ceilMult * nprime; /* smallest multiple of n' >= P */
    result = result * pow10d + n_big; /* append n and prepend 10^d */

    return result;
}
