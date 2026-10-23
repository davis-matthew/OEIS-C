/*
 * A021911  Decimal expansion of 1/907.
 * https://oeis.org/A021911
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n‑th digit of the decimal expansion of 1/907
 *            (0‑based). It is computed by long division: start with remainder
 *            r = 1; for each step digit = (r * 10) / 907; r = (r * 10) % 907
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised: as calls are made, digits up to the largest
   requested index are stored in a static array. */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* static cache of computed digits */
    static seq_t *digits = NULL;   /* array of digits (values 0‑9) */
    static size_t cap    = 0;      /* capacity of the array */
    static size_t size   = 0;      /* number of digits currently stored */
    static idx_t  rem    = 1;      /* current remainder in the long division */

    /* if the digit is already known, return it */
    if (n < size)
        return digits[n];

    /* ensure there is space for at least n+1 digits */
    ensure_cache(n, &digits, &cap);

    /* generate digits from current size up to n */
    while (size <= n)
    {
        /* next digit and new remainder */
        seq_t digit = (rem * 10) / 907;
        digits[size] = digit;

        /* prepare for next iteration */
        rem = (rem * 10) % 907;
        size++;
    }

    return digits[n];
}
