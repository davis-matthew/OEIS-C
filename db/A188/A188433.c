/*
 * A188433  a(n) = [2r]-[nr]-[2r-nr], where r=(1+sqrt(5))/2 and [.]=floor.
 * https://oeis.org/A188433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⎣2r⎦ – ⎣n r⎦ – ⎣2r – n r⎦, where r = (1+√5)/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 138, u64 n <= 138,
 *            u128 n <= 138, big any n
 * Validated: partial, pass, 138 terms through n = 138 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the identity ⎣nφ⎦ = ⎣(n + ⌊n√5⌋)/2⎦ and 2r = 1 + √5. */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Return ⎣n·φ⎦ (φ = (1+√5)/2). */
static seq_t floor_phi(idx_t n)
{
    seq_t nsq = (seq_t)n * (seq_t)n;   /* n² */
    seq_t t   = (seq_t)5 * nsq;        /* 5·n² */
    seq_t s   = floor_sqrt(t);         /* ⎣n√5⎦ */
    return ((seq_t)n + s) / 2;         /* ⎣(n + ⎣n√5⎦)/2⎦ = ⎣nφ⎦ */
}

/* OEIS A188433 */
seq_t a(idx_t n)
{
    /* Handles offset 1. */
    if (n == 1) return 1;
    if (n == 2) return 0;

    seq_t fn = floor_phi(n);          /* ⎣nφ⎦   */
    seq_t fk = floor_phi(n - 2);      /* ⎣(n‑2)φ⎦ */
    seq_t diff = fn - 4;              /* fn ≥ 4 for n ≥ 3 */
    return fk - diff;                 /* result is 0 or 1 */
}
