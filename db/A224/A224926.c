/*
 * A224926  Numerator of lexicographically least fraction f satisfying floor(f
 *          * 10^A055642(n)) = n.
 * https://oeis.org/A224926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = the minimal numerator a (1 ≤ a ≤ n) for which there
 *            exists a denominator b such that floor( a / b * 10^digits(n) ) =
 *            n. (digits(n) = number of decimal digits of n.)
 * Range:     u8 n <= 16, u16 n <= 1541, u32 n <= 9687, u64 n <= 9687,
 *            u128 n <= 9687, big any n
 * Helpers:   digits_of, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digits_of.h"   /* digits_of(idx_t n)  */
#include "../utils/pow10.h"       /* pow10(idx_t exp)    */

seq_t a(idx_t n)
{
    /* number of decimal digits of n */
    idx_t d = digits_of(n);

    /* 10^d as seq_t */
    seq_t pow10_d = pow10(d);

    seq_t a_candidate = 1;
    for (; a_candidate <= n; ++a_candidate)
    {
        /* M = a_candidate * 10^d (seq_t) */
        seq_t M = (seq_t)a_candidate * pow10_d;

        /* L = floor( M / (n+1) ) */
        seq_t L = M / (seq_t)(n + 1);

        /* R = floor( M / n ) */
        seq_t R = M / (seq_t)n;

        /* Condition: exists integer b with L < b ≤ R */
        if (L < R)
            return a_candidate;          /* minimal numerator found */
    }

    /* Theoretical guarantee: a must exist for 1 ≤ a ≤ n.
       If not found, return n as a fallback. */
    return (seq_t)n;
}
