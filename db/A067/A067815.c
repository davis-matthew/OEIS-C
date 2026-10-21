/*
 * A067815  a(n) = gcd(n, floor(sqrt(n))).
 * https://oeis.org/A067815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = gcd(n, floor(sqrt(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer (floor) square root for a 64‑bit unsigned value.
   Uses the bit‑shifting algorithm that works entirely with
   integer arithmetic. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t res = 0;
    idx_t bit = 1ULL << 62;          /* largest power of 4 <= UINT64_MAX */

    /* Bring bit down to the highest power of 4 <= n */
    while (bit > n)
        bit >>= 2;

    while (bit) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Euclidean algorithm for 64‑bit unsigned integers.  This
   routine is used directly so we do not depend on any external
   helper. */
static inline uint64_t gcd_u64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1 – the behaviour for 0 is irrelevant */
        return 0;

    idx_t root = isqrt_u64(n);
    uint64_t g   = gcd_u64((uint64_t)n, (uint64_t)root);

    return (seq_t)g;    /* cast to seq_t – values that fit will be correct */
}
