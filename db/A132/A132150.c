/*
 * A132150  Semi-chaotic Jazz function (Oscar Peterson robot function) on 13
 *          tones: a[n]->a[n-1]+/- 3 or 4 without 5 and 10 modulo 13.
 * https://oeis.org/A132150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, hear
 * Formula:   a(n) = h(n) where f(n) = n + p(n), p(n) periodic with period 6:
 *            4, 9, -2, 3, 10, -3 g(n) = (f(n) > 12) ? 1 + f(n) mod 13 : f(n)
 *            a(n) = (g(n) % 5 == 0) ? g(n) - 1 : g(n) (all values are between
 *            1 and 13)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* periodic delta values: 4, 9, -2, 3, 10, -3 for n = 1..6 */
    static const int period[6] = {4, 9, -2, 3, 10, -3};
    int p = period[(int)((n - 1) % 6ULL)];          /* signed shiftper */

    /* For small n we can compute f exactly; otherwise use modular arithmetic. */
    if (n <= 15) {
        int64_t f = (int64_t)n + (int64_t)p;
        int64_t g;
        if (f > 12) {
            int64_t r = f % 13;      /* 0 .. 12 */
            g = 1 + r;               /* 1 .. 13 */
        } else {
            g = f;                   /* 1 .. 12 */
        }
        seq_t res = (seq_t)g;
        if (g % 5 == 0)               /* adjust for multiples of 5 */
            res = (seq_t)(g - 1);
        return res;
    } else {
        /* For large n, f > 12, so g = 1 + (n + p) mod 13.  Compute modulo safely. */
        int64_t r = (int64_t)(n % 13ULL) + (int64_t)p;  /* small intermediate value */
        r %= 13;                                   /* normalise to 0..12, may be negative */
        if (r < 0) r += 13;                        /* make positive remainder */
        int64_t g = 1 + r;                         /* g in 1 .. 13 */
        seq_t res = (seq_t)g;
        if (g % 5 == 0)                           /* adjust for multiples of 5 */
            res = (seq_t)(g - 1);
        return res;
    }
}
