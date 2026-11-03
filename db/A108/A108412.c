/*
 * A108412  Expansion of (1 + x + x^2)/(1 - 4x^2 + x^4).
 * https://oeis.org/A108412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - a(n-4) with a(0)=1, a(1)=1, a(2)=5, a(3)=4
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 998 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* safe multiplication that does not overflow for unsigned types up to 64 bits */
static inline seq_t mul_safe(seq_t a, unsigned b)
{
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        unsigned __int128 tmp = (unsigned __int128)a * b;
        /* maximum value representable in seq_t */
        seq_t maxv = (seq_t)(~(seq_t)0);
        return tmp > maxv ? maxv : (seq_t)tmp;
    }
    /* arbitrary‑precision types handle overflow internally */
    return a * (seq_t)b;
}

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n == 0u) return 1;
    if (n == 1u) return 1;
    if (n == 2u) return 5;
    if (n == 3u) return 4;

    /* Sliding window: a0=a(i-4), a1=a(i-3), a2=a(i-2), a3=a(i-1) */
    seq_t a0 = 1;   /* a(i-4) */
    seq_t a1 = 1;   /* a(i-3) */
    seq_t a2 = 5;   /* a(i-2) */
    seq_t a3 = 4;   /* a(i-1) */
    seq_t new_val;

    for (idx_t i = 4; i <= n; ++i) {
        /* a(i) = 4*a(i-2) - a(i-4) */
        new_val = mul_safe(a2, 4u) - a0;

        /* Shift the window for the next step */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = new_val;
    }

    return a3;  /* a(n) is stored in a3 after the loop */
}
