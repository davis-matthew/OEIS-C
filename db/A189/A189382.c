/*
 * A189382  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=-1+sqrt(2), t=1+sqrt(2).
 * https://oeis.org/A189382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n/(1+√2)⌋ + ⌊n*(-1+√2)/(1+√2)⌋
 * Range:     u8 n <= 5, u16 n <= 1271, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer‑only computation below. */

#include <stdint.h>

static inline seq_t isqrt_seq(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        /* avoid overflow: mid*mid <= x   <=>   mid <= x/mid */
        if (mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Ensure n >= 1; behaviour for n==0 is irrelevant. */
    if (n == 0)
        return 0;

    seq_t fn = (seq_t)n;
    /* floor(sqrt(2) * n)   == floor(sqrt(2 * n^2)) */
    seq_t s = isqrt_seq((seq_t)2 * fn * fn);

    /* floor(2 * sqrt(2) * n) == floor(sqrt(8 * n^2))   (used to test fractional part) */
    seq_t ssq = isqrt_seq((seq_t)8 * fn * fn);
    seq_t two_s = s << 1;          /* 2 * floor(n*sqrt2) */

    /* delta = 1 if fractional part <= 0.5, else 2   (see analysis) */
    seq_t delta = (ssq == two_s) ? 1 : 2;

    /* a(n) = 3n - floor(n*sqrt2) - delta */
    return 3 * fn - s - delta;
}
