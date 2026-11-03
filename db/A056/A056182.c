/*
 * A056182  First differences of A003063.
 * https://oeis.org/A056182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 994 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A056182 – first differences of A003063
    a(0) = 0
    a(1) = 2
    a(n) = 5*a(n-1) – 6*a(n-2)      (for n >= 2)
    (equivalently, a(n) = 2*(3^n – 2^n)) */

#include <stdint.h>
#include <stdlib.h>

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;

    seq_t prev2 = (seq_t)0;       /* a(0) */
    seq_t prev1 = (seq_t)2;       /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term1;
        seq_t term2;

#if defined(__SIZEOF_INT128__) && !defined(_HAS_NO_INT128)
        /* Use native 128‑bit arithmetic for builtin integer types
         * to avoid overflow during the intermediate multiplication.
         * This path is taken only when seq_t is 64 bit or smaller. */
        if (sizeof(seq_t) <= sizeof(unsigned long long)) {
            __int128 t1 = (__int128)prev1 * 5;
            __int128 t2 = (__int128)prev2 * 6;
            cur = (seq_t)(t1 - t2);
        } else
#endif
        {
            /* For arbitrary‑precision seq_t (or when 128‑bit isn’t available),
             * use the helper that multiplies by a small integer safely. */
            term1 = mul_small(prev1, 5);
            term2 = mul_small(prev2, 6);
            cur = term1 - term2;
        }

        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
