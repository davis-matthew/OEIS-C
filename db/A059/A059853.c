/*
 * A059853  Period of continued fraction for sqrt(n^2+3), n >= 2.
 * https://oeis.org/A059853
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n mod 3 == 0; otherwise the period of the continued
 *            fraction for sqrt(n²+3). Computed by the standard CF algorithm
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>     /* for bool (if needed) */

/* integer square root for the type seq_t */
static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    while (low <= high) {
        seq_t mid  = (low + high) / 2;
        seq_t mid2 = mid * mid;
        if (mid2 == x) return mid;
        if (mid2 < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* 1. trivial period for multiples of 3 */
    if (n % 3ULL == 0ULL)
        return (seq_t)2;

    /* 2. D = n² + 3 as seq_t */
    seq_t nseq  = (seq_t)n;
    seq_t D     = nseq * nseq + (seq_t)3;

    /* 3. integer sqrt of D */
    seq_t a0 = isqrt_seq(D);

    /* 4. continued‑fraction loop */
    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;
    seq_t period = 0;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        period++;

        /* period ends when a == 2*a0 */
        if (a == 2 * a0)
            break;
    }
    return period;
}
