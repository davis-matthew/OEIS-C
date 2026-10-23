/*
 * A025848  Expansion of 1/((1-x^3)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A025848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 21*n + 135)/462 + (5/42)*(((n+2) mod 3) - (n
 *            mod 3)) + ((2*n^2 + 6) mod 7)/7)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 64‑bit signed integer for all intermediate calculations.
       The expression is proven to be non‑negative for all n >= 0,
       so the final division is safe. */
    uint64_t nu = (uint64_t)n;                /* base value of n */
    int64_t  N   = (int64_t)nu * (int64_t)nu      /* n^2          */
                 + 21LL * (int64_t)nu            /* + 21*n       */
                 + 135LL;                       /* + 135        */

    /* delta = ((n+2) mod 3) - (n mod 3)  (range -2..2) */
    int64_t delta = (int64_t)((nu + 2ULL) % 3ULL) - (int64_t)(nu % 3ULL);

    /* r = (2*n^2 + 6) mod 7  (range 0..6) */
    int64_t r = (int64_t)((2ULL * (nu * nu) + 6ULL) % 7ULL);

    /* Combine all terms with common denominator 462.
       Numerator = N + 55*delta + 66*r   */
    int64_t numer = N + 55LL * delta + 66LL * r;

    /* integer division truncates toward zero; numer is guaranteed
       to be non‑negative, thus it is equivalent to floor. */
    return (seq_t)(numer / 462LL);
}
