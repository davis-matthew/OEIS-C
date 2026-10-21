/*
 * A075119  Denominator of n/floor(sqrt(n)); numerator is A073890(n).
 * https://oeis.org/A075119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = floor(sqrt(n)) / gcd(n, floor(sqrt(n)))
 * Range:     u8 n <= 2981, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the denominator of the reduced fraction
   n / floor(sqrt(n)).  It works for any seq_t/idx_t pair
   supported by the build and requires no external tables. */

#include "../utils/gcd.h"

/* Compute floor(sqrt(n)) for an unsigned 64‑bit integer n.
   Uses only integer arithmetic and works for all n. */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        /* avoid overflow: mid > n/mid iff mid*mid > n */
        if (mid > 0 && mid > n / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;           /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n<1, but guard just in case */
        return (seq_t)0;

    idx_t f = floor_sqrt_u64(n);            /* floor(sqrt(n)) */
    seq_t sn = (seq_t)n;                    /* numerator n        */
    seq_t sf = (seq_t)f;                    /* denominator f      */
    seq_t g  = gcd(sn, sf);                 /* gcd(n, f)          */
    return sf / g;                          /* reduced denominator*/
}
