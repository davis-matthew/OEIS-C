/*
 * A081917  a(0)=1, a(n)= n^(n-2)(7n^2-3n+2)/6 (n>0).
 * https://oeis.org/A081917
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A081917: a(n) = n^(n-2) * (7 n^2 - 3 n + 2) / 6  (n > 0), a(0)=1, a(1)=1 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* compute n^(n-2) by binary exponentiation */
    idx_t exp = n - 2;        /* exponent is non‑negative for n >= 2 */
    seq_t base = (seq_t)n;
    seq_t power = (seq_t)1;
    while (exp > 0) {
        if (exp & 1UL)
            power *= base;
        exp >>= 1;
        if (exp > 0)         /* avoid one unnecessary squaring */
            base *= base;
    }

    /* polynomial factor 7n^2 - 3n + 2 */
    seq_t poly = ((seq_t)7 * (seq_t)n * (seq_t)n) - ((seq_t)3 * (seq_t)n) + (seq_t)2;

    /* final value */
    return (power * poly) / (seq_t)6;
}
