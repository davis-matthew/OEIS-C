/*
 * A168658  a(n) = ceiling(n^n/2).
 * https://oeis.org/A168658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(n^n/2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 0^0 is defined as 1 in this sequence */
    if (n == 0)
        return 1;

    /* 64‑bit seq_t: use 128‑bit temporaries to avoid overflow
       up to n = 16 (large n will overflow naturally; correctness is
       only required for values that fit in seq_t). */
    if (sizeof(seq_t) == 8) {
        unsigned __int128 p = 1;
        for (idx_t i = 1; i <= n; ++i)
            p *= (unsigned __int128)(seq_t)n;

        seq_t r = (seq_t)(p / 2);
        if (p & 1u) /* odd value – add one after truncating division */
            ++r;
        return r;
    }

    /* For arbitrary‑precision or smaller types use the generic power routine.
       Correctness for out‑of‑range values is not required. */
    seq_t v = big_pow((seq_t)n, (seq_t)n);   /* compute n^n */
    seq_t r = v / 2;
    if (v % 2) /* odd remainder */
        ++r;
    return r;
}
