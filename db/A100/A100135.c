/*
 * A100135  a(n) = Sum_{k=0..floor(n/6)} C(n-3k,3k) * 2^k.
 * https://oeis.org/A100135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 83, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A100135: a(n) = Sum_{k=0..⌊n/6⌋} C(n-3k,3k) * 2^k
   Recurrence: a(0..5)=1, a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + 2*a(n-6) for n≥6 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence is constant 1 for n = 0…5 */
    if (n < 6)
        return 1;

    /* Sliding‑window variables: a0…a5 = a(n‑6)…a(n‑1) */
    seq_t a0 = 1, a1 = 1, a2 = 1, a3 = 1, a4 = 1, a5 = 1, an;

    for (idx_t i = 6; i <= n; ++i) {
        an = 3 * a5 - 3 * a4 + a3 + 2 * a0;
        /* shift forward: drop a0, advance the window */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }
    return an;
}
