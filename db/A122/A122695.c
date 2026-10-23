/*
 * A122695  Number of edges in the n-th Mycielski graph.
 * https://oeis.org/A122695
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (18 - 27*2^n + 14*3^n)/36 (for n ≥ 0) Initial values:
 *            a(0)=0, a(1)=0, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 21, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Special cases that match the sequence definition. */
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    /* If the underlying integer type is at most 64 bits, use a 128‑bit
       intermediate to avoid overflow when evaluating the numerator. */
    if (sizeof(seq_t) <= 8) {
        /* Compute 2^n and 3^n using a 128‑bit integer. The range of n for
           which the result fits into a 64‑bit container is well below 128,
           so the shift and loop are safe. */
        __uint128_t pow2 = ((__uint128_t)1) << n;   /* 2^n */
        __uint128_t pow3 = 1;
        for (idx_t i = 0; i < n; ++i)
            pow3 *= 3;                               /* 3^n */

        __uint128_t num = (__uint128_t)18
                         - (__uint128_t)27 * pow2
                         + (__uint128_t)14 * pow3;

        return (seq_t)(num / 36);
    }

    /* For arbitrary‑precision seq_t, use the provided power helpers. */
    seq_t p2 = pow2(n);   /* 2^n as seq_t */
    seq_t p3 = pow3(n);   /* 3^n as seq_t */

    seq_t numer = 18;
    numer -= 27 * p2;
    numer += 14 * p3;

    return numer / 36;
}
