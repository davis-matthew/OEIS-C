/*
 * A093299  a(n) is the largest number such that all of a(n)'s length-n
 *          substrings are distinct and divisible by 99.
 * https://oeis.org/A093299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 0; a(2) = 99; a(2k+1) = 10^(2k+1) - 10 (n odd); a(2k) =
 *            10^(4k-1) - 10^(2k+1) + 10^(2k-1) - 10 (k >= 2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 468 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 0;
    if (n == 2)
        return 99;

    if (n & 1)  /* odd n = 2k+1 */
    {
        seq_t val = pow10(n);   /* 10^n */
        val -= 10;
        return val;
    }

    /* even n >= 4: n = 2k, k >= 2 */
    idx_t k = n / 2;
    seq_t val = pow10(4 * k - 1);          /* 10^(4k-1) */
    val -= pow10(2 * k + 1);               /* -10^(2k+1) */
    val += pow10(2 * k - 1);               /* +10^(2k-1) */
    val -= 10;                             /* -10 */
    return val;
}
