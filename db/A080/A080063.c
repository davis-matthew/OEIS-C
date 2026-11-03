/*
 * A080063  a(n) = n mod (lpf(n)+1), where lpf(n) is the least prime dividing
 *          n (A020639).
 * https://oeis.org/A080063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n mod (lpf(n)+1) where lpf(n) is the least prime divisor
 *            of n
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Validated: partial, pass, 96 terms through n = 96 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes on-demand; works for any idx_t and seq_t. */

#include <stdint.h>

/* Return the least prime factor of n (n >= 1).  For n = 1, returns 1. */
static inline idx_t lpf_idx(idx_t n)
{
    if (n <= 1)
        return 1;                     /* special case for n == 1   */
    if ((n & 1) == 0)                /* check even numbers        */
        return 2;
    for (idx_t p = 3; p * p <= n; p += 2)
        if (n % p == 0)
            return p;                /* first divisor is the lpf  */
    return n;                         /* n is prime                 */
}

seq_t a(idx_t n)
{
    idx_t p = lpf_idx(n);
    return (seq_t)n % ((seq_t)p + 1);
}
