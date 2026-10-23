/*
 * A145009  Array read by antidiagonals: array of odd integers found in
 *          |A144912| with axes b = {4, 6, 8, ...} and n = {b^2, b^4, b^6,
 *          ...}.
 * https://oeis.org/A145009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A145009 as “Array read by antidiagonals”.
   Formula (derived from the array definition  
   A(n, k) = 6*(n + k) + 4*n*k + 7):
   For a given index n let s be the unique integer such that

        s(s + 1)/2 ≤ n < (s + 1)(s + 2)/2

   and set t = n – s(s + 1)/2.  The value a(n) is

        a(n) = 6*s + 4*t*(s – t) + 7
*/
#include <stdint.h>

static idx_t diagonal(idx_t n, idx_t *t_out)
{
    idx_t low = 0, high = 1;

    /* Find an upper bound for s by doubling */
    while (1) {
        __uint128_t val = (__uint128_t)high * (high + 1) / 2;
        if (val > n) break;
        high <<= 1;
    }

    /* Binary search the exact s */
    while (high > low + 1) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid;
    }

    *t_out = n - (idx_t)((__uint128_t)low * (low + 1) / 2);
    return low;                 /* s */
}

seq_t a(idx_t n)
{
    idx_t s, t;
    s = diagonal(n, &t);        /* s = floor((sqrt(8n+1)-1)/2), t = n - s(s+1)/2 */

    __uint128_t value = (__uint128_t)6 * s
                    + (__uint128_t)4 * t * (s - t)
                    + 7;

    return (seq_t)value;
}
