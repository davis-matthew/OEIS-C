/*
 * A238529  a(0) = a(1) = 0, and for n > 1, a(n) = number of iterations of
 *          A238525 (n modulo sopfr(n)) needed to reach either 0 or 1. Here
 *          sopfr(n) is the sum of the prime factors of n, with multiplicity,
 *          A001414.
 * https://oeis.org/A238529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9362, u16 n <= 9362, u32 n <= 9362, u64 n <= 9362,
 *            u128 n <= 9362, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=0; for n>1,
   a(n)=1 + a(n % sopfr(n)),
   where sopfr(n) is the sum of the prime factors of n with multiplicity. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Compute sopfr(n) for n ≥ 0.
   The value never exceeds n (and n is uint64_t), so a 64‑bit accumulator is
   sufficient.  Trial division is fast enough for n up to at least 10⁶. */
static inline uint64_t sopfr_uint64(uint64_t n)
{
    if (n < 2)
        return 0;                 /* sopfr(0) and sopfr(1) are 0 by convention */

    uint64_t sum = 0;
    uint64_t t   = n;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        while (t % p == 0)
        {
            sum += p;
            t   /= p;
        }
    }
    if (t > 1)
        sum += t;                  /* remaining prime factor */
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                 /* a(0) = a(1) = 0 */

    uint64_t sp = sopfr_uint64(n);    /* sum of prime factors, fits in uint64_t */
    uint64_t m  = n % sp;             /* n modulo sopfr(n) */

    seq_t depth = a((idx_t)m);        /* recursive call */
    return depth + 1;                 /* a(n) = 1 + a(m) */
}
