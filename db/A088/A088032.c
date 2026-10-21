/*
 * A088032  Smallest number k such that k^n -1 is divisible by an n-th power.
 *          a(n) = A088031(n)^(1/n).
 * https://oeis.org/A088032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn
 * Formula:   a(n) = 2^n + 1 (for odd n, n ≥ 3); a(n) = 2^(n - v2(n)) - 1 (for
 *            even n, n ≠ 2); a(1) = a(2) = 3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  v2(n) is the exponent of 2 in n (largest k such that 2^k divides n). */

#include <stdint.h>
#include <stdbool.h>

static seq_t pow2_exp(idx_t e)
{
    seq_t r = (seq_t)1;
    /* multiply by 2 e times, safe for arbitrary precision seq_t */
    while (e--) r = r + r;  /* = r*2 */
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)3;
    if (n & 1) { /* odd */
        seq_t p = pow2_exp((idx_t)n);
        return p + (seq_t)1;
    } else {
        /* compute v2(n) */
        idx_t v = 0;
        idx_t t = n;
        while ((t & 1) == 0) {
            ++v;
            t >>= 1;
        }
        seq_t p = pow2_exp((idx_t)(n - v));
        return p - (seq_t)1;
    }
}
