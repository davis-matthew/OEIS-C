/*
 * A094918  a(n) = (3^n-1)/2 mod n.
 * https://oeis.org/A094918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (3^n - 1)/2 mod n (offset 1)
 * Range:     u8 n <= 287, u16 n <= 9055, u32 n <= 9055, u64 n <= 9055,
 *            u128 n <= 9055, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For safety, handle n = 0 although the sequence is defined for n ≥ 1. */
    if (n == 0)
        return 0;

    /* Compute 3^n modulo 2n.  Since n ≤ 10000, 2n < 20000, so 64‑bit
       integers are more than sufficient for the intermediate arithmetic. */
    uint64_t mod   = (uint64_t)n * 2ULL;
    uint64_t rval  = 1ULL;           /* 3^0 == 1 */
    uint64_t base  = 3ULL % mod;     /* 3 mod 2n */
    uint64_t exp   = (uint64_t)n;

    while (exp > 0) {
        if (exp & 1ULL)
            rval = (rval * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }

    /* (3^n - 1)/2 is guaranteed to be an integer and < n, so we can
       safely cast it to seq_t and return it directly. */
    seq_t result = (seq_t)((rval - 1ULL) >> 1ULL); /* (rval-1)/2 */
    return result;
}
