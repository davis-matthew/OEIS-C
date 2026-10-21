/*
 * A065559  Smallest k such that tau(n+k) = tau(k).
 * https://oeis.org/A065559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = smallest k ≥ 1 such that τ(n+k) = τ(k) (τ = divisor
 *            count)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   tau64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tau64.h"

/* --------------------------------------------------------------------
   Main sequence function.
   An arbitrary-precision seq_t is used for the return value, so we
   cast the 64‑bit k into seq_t at the end.  The function works for
   any idx_t n ≥ 1, regardless of order of calls.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety guard – the sequence is defined for n ≥ 1 */

    idx_t k = 1;
    for (;; ++k)
    {
        idx_t nk = n + k;
        if (tau64(nk) == tau64(k))
            return (seq_t)k;
    }
}
