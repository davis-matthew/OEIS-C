/*
 * A010340  Sum of terms in period of continued fraction for sqrt(n).
 * https://oeis.org/A010340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = sum of the periodic part of the continued fraction
 *            expansion of sqrt(n); a(n)=0 if n is a perfect square
 * Range:     u8 n <= 318, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for 64‑bit unsigned n. */
static inline idx_t sqrt_u64(idx_t n)
{
    idx_t low = 0, high = (n >> 1) + 1;   /* high > sqrt(n) */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if ( (mid > 0) && (mid > n / mid) ) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t s = sqrt_u64(n);
    if ((seq_t)s * (seq_t)s == n)   /* perfect square → 0 */
        return 0;

    seq_t a0   = (seq_t)s;    /* floor(sqrt(n))  */
    seq_t m    = 0;
    seq_t d    = 1;
    seq_t term = a0;          /* current a_i  */
    seq_t sum  = 0;

    while (1) {
        m = d * term - m;

        seq_t temp = (seq_t)n - m * m;
        d = temp / d;

        term = (a0 + m) / d;

        sum += term;
        if (term == 2 * a0)   /* end of period */
            break;
    }
    return sum;
}
