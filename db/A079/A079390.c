/*
 * A079390  a(n) = lpf(n) * lpf(n+1) * lpf(n+2), where lpf(n) = A020639(n) is
 *          the least prime factor of n.
 * https://oeis.org/A079390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lpf(n) * lpf(n+1) * lpf(n+2) (lpf(1) = 1)
 * Range:     u8 n <= 10, u16 n <= 190, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Least‑prime‑factor routine for 64‑bit integers.
 * Works for all values that appear in the sequence when using seq_t
 * of any size (the returned value is cast to seq_t). */
static seq_t lpf_uint64(uint64_t m)
{
    if (m <= 1)                 return (seq_t)1;   /* defined as 1 in OEIS */
    if ((m & 1ULL) == 0)        return (seq_t)2;   /* even numbers */

    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d == 0)         return (seq_t)d;   /* first divisor found */
    }
    return (seq_t)m;            /* m is prime */
}

/* Sequence function a(n).  idx_t is an unsigned 64‑bit integer. */
seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    seq_t l1 = lpf_uint64(N);
    seq_t l2 = lpf_uint64(N + 1);
    seq_t l3 = lpf_uint64(N + 2);
    return l1 * l2 * l3;        /* arithmetic works for any seq_t, including big integers */
}
