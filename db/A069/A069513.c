/*
 * A069513  Characteristic function of the prime powers p^k, k >= 1.
 * https://oeis.org/A069513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 if n is a prime power (n = p^k, k ≥ 1), otherwise 0;
 *            a(1) = 0
 * Range:     u8 n <= 9257, u16 n <= 9257, u32 n <= 9257, u64 n <= 9257,
 *            u128 n <= 9257, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a simple trial‑division factorisation.  For
   n up to 10 000 this runs well under 2 ms per call and does not
   rely on any external helper functions.  The function works for any
   larger n that fits in idx_t, with an O(√n) time complexity. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2U)
        return (seq_t)0;                 /* a(1) = 0 */

    uint64_t m = (uint64_t)n;            /* work with a 64‑bit copy */
    int distinct = 0;                    /* distinct prime factors */

    for (uint64_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            ++distinct;
            do
                m /= p;
            while (m % p == 0);
        }
    }

    if (m > 1)
        ++distinct;                       /* remaining prime > √original */

    return (seq_t)(distinct == 1);        /* 1 if exactly one prime factor */
}
