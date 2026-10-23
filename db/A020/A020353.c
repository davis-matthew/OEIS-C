/*
 * A020353  Numbers k such that the continued fraction for sqrt(k) has period
 *          14.
 * https://oeis.org/A020353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1709, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers k such that the continued‑fraction period of sqrt(k) equals 14. */

#include <stdint.h>
#include <stdbool.h>

/* ----- integer square root (floor) for 64 bit unsigned values ----- */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t x = n >> 1;
    while (x > n / x)
        x = (x + n / x) >> 1;
    return x;
}

/* ----- continued‑fraction period of sqrt(k) (k > 0, not a perfect square) ----- */
static int period_of_sqrt_u64(idx_t k)
{
    idx_t a0 = isqrt_u64(k);
    if (a0 * a0 == k)      /* perfect square – period is zero */
        return 0;

    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;
    int len = 0;

    do {
        /* m = d*a - m  (use 128‑bit to avoid overflow) */
        __uint128_t prod = (__uint128_t)d * a;
        m = (idx_t)(prod - m);

        /* d = (k - m*m)/d  (m*m fits in 64 bits) */
        idx_t mm = (idx_t)(((__uint128_t)m * m));
        d = (k - mm) / d;

        /* a = (a0 + m)/d */
        a = (a0 + m) / d;
        len++;

        /* quick exit – we only care about period 14 */
        if (len > 14)
            return len;
    } while (a != 2 * a0);

    return len;
}

/* ----- public function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t k = 2;                /* start checking from 2 */

    while (true) {
        if (period_of_sqrt_u64(k) == 14) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
}
