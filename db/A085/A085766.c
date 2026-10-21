/*
 * A085766  Smallest m such that n divides the tetrahedral number
 *          A000292(m+1).
 * https://oeis.org/A085766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) – the least m≥1 such that n divides the tetrahedral number
   T(m+1) = (m+1)(m+2)(m+3)/6 */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return 1;                       /* a(1)=a(2)=1 */

    /* 6*n must fit in uint64_t for the modulo calculations */
    uint64_t M = 6 * n;                  /* M = 6n */

    for (uint64_t m = 1; ; ++m) {
        uint64_t a1 = m + 1;
        uint64_t a2 = m + 2;
        uint64_t a3 = m + 3;

        /* compute ((a1 * a2) % M * a3) % M using 128‑bit intermediates
           to avoid overflow */
        uint64_t p1 = (uint64_t)(((__uint128_t)a1 * a2) % M);
        uint64_t prod = (uint64_t)(((__uint128_t)p1 * a3) % M);

        if (prod == 0)
            return (seq_t)m;
    }
    /* unreachable */
}
