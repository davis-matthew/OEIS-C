/*
 * A215817  a(n) is the rational part of A(n) = (6-sqrt(7))*A(n-1) -
 *          (12-4*sqrt(7))*A(n-2) + (8-3*sqrt(7))*A(n-3) with A(0)=3,
 *          A(1)=6-sqrt(7), A(2)=19-4*sqrt(7).
 * https://oeis.org/A215817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rational part of A(n) where A(n) = (6‑sqrt(7))*A(n‑1) –
 *            (12‑4*sqrt(7))*A(n‑2) + (8‑3*sqrt(7))*A(n‑3) with A(0)=3,
 *            A(1)=6‑sqrt(7), A(2)=19‑4*sqrt(7)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   cast_to_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We keep both the rational part a(n) and the
   sqrt(7) coefficient b(n) (which may be negative).
   Recurrence for the pair (a,b) is derived from
   the above (exact integer arithmetic).  For any
   width of seq_t we compute the pair in signed
   128‑bit, which is sufficient for all n for
   which a(n) fits into seq_t. */

#include <stdint.h>
#include "../utils/cast_to_seq_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)19;

    /* Use signed __int128 for intermediate values.
       For arbitrary‑precision seq_t this cast works,
       for built‑in uintXX_t it holds values that fit in
       __int128 (which covers all n for which a(n) fits
       into seq_t). */
    __int128 a0 = 3, a1 = 6, a2 = 19;       /* a(n‑3), a(n‑2), a(n‑1) */
    __int128 b0 = 0, b1 = -1, b2 = -4;      /* b(n‑3), b(n‑2), b(n‑1) */

    for (idx_t i = 3; i <= n; ++i)
    {
        __int128 anew =  6*a2 - 7*b2 -12*a1 + 28*b1 + 8*a0 - 21*b0;
        __int128 anew2;   /* Not used, kept for 128-bit safety */
        __int128 bnew =  6*b2 - a2  -12*b1 + 4*a1  + 8*b0 - 3*a0;

        /* Shift the window */
        a0 = a1; a1 = a2; a2 = anew;
        b0 = b1; b1 = b2; b2 = bnew;
    }

    return cast_to_seq_t(a2);
}
