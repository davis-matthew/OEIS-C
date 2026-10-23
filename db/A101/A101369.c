/*
 * A101369  a(2n-1) = the smallest positive integer not occurring earlier in
 *          the sequence. a(2n) = the a(2n-1)th smallest positive integer
 *          among those not occurring earlier in the sequence.
 * https://oeis.org/A101369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(2n-1) = 1 + ⌊(2n-1)/√2⌋ a(2n) = 2n + ⌊(2n-1)/√2⌋ The
 *            fractional part of (2n-1)/√2 is handled via integer arithmetic:
 *            ⌊k/√2⌋ = ⌊⌊k√2⌋ / 2⌋ and ⌊k√2⌋ is the integer part of √(2·k²),
 *            therefore ⌊k/√2⌋ = isqrt(2·k²) >> 1
 * Range:     u8 n <= 12, u16 n <= 399, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation relies on the provided integer square‑root
   routine isqrt from the utils library. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt.h"

extern seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n ≤ 0, return 0 */
        return 0;

    /* compute k = 2n-1 (used in both odd and even cases) */
    seq_t k;
    if (n & 1U) {        /* odd index: n = 2m-1 → m = (n+1)/2 */
        k = (seq_t)n;    /* k = 2n-1 for odd n is just n itself */
    } else {             /* even index: n = 2m → m = n/2 */
        k = (seq_t)(n - 1); /* k = 2m-1 = n-1 */
    }

    /* Δ = ⌊k/√2⌋ = isqrt(2·k²) >> 1 */
    seq_t kk  = k * k;            /* k²  */
    seq_t t   = (seq_t)2 * kk;    /* 2·k² */
    seq_t delta = isqrt(t) >> 1;  /* divide by 2 */

    if (n & 1U) {        /* odd n */
        return (seq_t)1 + delta;
    } else {             /* even n */
        return (seq_t)n + delta;
    }
}
