/*
 * A189179  Number of n X 2 array permutations with each element making a
 *          single king move.
 * https://oeis.org/A189179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 4*a(n-2) - 16*a(n-3) a(1) = 1, a(2) = 9, a(3)
 *            = 33
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 199 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    /* The sequence is defined only for n >= 1 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)33;

    seq_t a1 = (seq_t)1;   /* a(n-3) */
    seq_t a2 = (seq_t)9;   /* a(n-2) */
    seq_t a3 = (seq_t)33;  /* a(n-1) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t term = mul_small(a3, 5);        /* 5 * a(n-1) */
        term += mul_small(a2, 4);             /* + 4 * a(n-2) */
        term -= mul_small(a1, 16);            /* - 16 * a(n-3) */

        a1 = a2;
        a2 = a3;
        a3 = term;
    }
    return a3;
}
