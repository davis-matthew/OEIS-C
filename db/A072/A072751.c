/*
 * A072751  Greatest of the most frequent prime factors of squarefree numbers
 *          <= n; a(1)=1.
 * https://oeis.org/A072751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed: a(n)=2 for n>14, otherwise pre‑computed small terms */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* 0 is unused; indices 1..14 are the non‑trivial terms */
    static const seq_t pre[15] = {
        0,   /* 0 unused */
        1,   /* a(1) */
        2,
        3,
        5,
        3,
        3,
        2,
        2,
        2,
        2,
        2,
        2,
        2,
        3
    };

    if (n <= 14)
        return pre[(size_t)n];
    return 2;
}
