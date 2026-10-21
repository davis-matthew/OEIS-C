/*
 * A081910  a(n) = 4^n*(n^2 - n + 32)/32.
 * https://oeis.org/A081910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n*(n^2 - n + 32)/32
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For seq_t small (≤ 64 bits) the expression is evaluated with 128‑bit intermediates
   to avoid overflow.  For arbitrary‑precision or 128‑bit seq_t a simple linear
   recurrence is used, which is exact for all term sizes. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* --- fast path for small integer seq_t types (≤ 64 bits) --- */
    if (sizeof(seq_t) <= 8) {
        /* Use unsigned 128‑bit arithmetic (supported by GCC/Clang). */
        unsigned __int128 pow4 = 1;          /* 4^n */
        for (idx_t i = 0; i < n; ++i)
            pow4 *= 4;

        unsigned __int128 t = (__uint128_t)n * n - n + 32; /* n^2 - n + 32 */

        unsigned __int128 prod = pow4 * t;  /* intermediate product  */
        unsigned __int128 res  = prod >> 5; /* divide by 32 */

        return (seq_t)res;
    }

    /* --- fallback for arbitrary‑precision or 128‑bit seq_t --- */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)17;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)4;   /* a(n-2)   */
    seq_t a2 = (seq_t)17;  /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t t1  = a2 * 12;   /* 12*a(n-1) */
        seq_t t3  = a0 * 64;   /* 64*a(n-3) */
        seq_t sum = t1 + t3;   /* 12*a(n-1)+64*a(n-3) */
        seq_t res = sum - a1 * 48;   /* subtract 48*a(n-2) */

        a0 = a1;
        a1 = a2;
        a2 = res;
    }
    return a2;
}
