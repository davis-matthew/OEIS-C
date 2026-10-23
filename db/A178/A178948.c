/*
 * A178948  a(n) = smallest m>=0 such that n*(3^m)-1 is prime, or -1 if no
 *          such prime exists.
 * https://oeis.org/A178948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 21, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find smallest m ≥ 0 such that n*3^m – 1 is prime;
   return –1 if no such prime exists for the given n. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* For odd n ≥ 5 the expression n·3^m – 1 is even and > 2,
       hence never prime. */
    if ((n & 1) && n >= 5)
        return (seq_t)-1;

    /* Maximum representable positive value of type seq_t. */
    const __int128 max_val = (((__int128)1) << (sizeof(seq_t) * 8 - 1)) - 1;

    seq_t pow3 = 1;          /* 3^m */
    idx_t  m   = 0;

    for (;; ++m)
    {
        /* Compute n * 3^m in a wider type to detect overflow. */
        __int128 prod = (__int128)n * (__int128)pow3;
        if (prod > max_val)          /* would exceed seq_t range */
            break;

        seq_t val = (seq_t)(prod - 1);
        if (is_prime_big(val))
            return (seq_t)m;

        /* Prepare next power of 3, checking that the product stays
           within the range of seq_t. */
        if ((__int128)pow3 > max_val / 3)
            break;                  /* pow3 * 3 would overflow seq_t */
        pow3 *= 3;
    }

    return (seq_t)-1;
}
