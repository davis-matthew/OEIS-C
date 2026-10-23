/*
 * A104806  "Round of hypotenuse", see comments.
 * https://oeis.org/A104806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000194(A022099(n-1))
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: let t₁ = 1, t₂ = 9 and for k ≥ 3: t_k = t_{k-1}+t_{k-2}.    */
/* Then a(n) = round(√t_n).  Compute the floor of the square root (isqrt)   */
/* and round up if t_n ≥ (floor_sqrt)² + floor_sqrt + 1. */

#include "../utils/isqrt.h"

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* Compute t_n via Fibonacci‑type recurrence */
    seq_t t_prev2 = (seq_t)1; /* t₁ */
    seq_t t_prev1 = (seq_t)9; /* t₂ */
    seq_t t_cur   = (seq_t)0;
    for (idx_t i = 3; i <= n; ++i) {
        t_cur   = t_prev1 + t_prev2;
        t_prev2 = t_prev1;
        t_prev1 = t_cur;
    }

    /* t_cur now holds t_n.  Compute round(√t_n). */
    seq_t y   = isqrt(t_cur);                 /* floor(√t_n) */
    seq_t y2  = y * y;                        /* y² */
    seq_t lim = y2 + y + (seq_t)1;            /* y² + y + 1 */
    if (t_cur >= lim)
        return y + (seq_t)1;                  /* round up */
    else
        return y;                            /* round down */
}
