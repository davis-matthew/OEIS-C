/*
 * A188433  a(n) = [2r]-[nr]-[2r-nr], where r=(1+sqrt(5))/2 and [.]=floor.
 * https://oeis.org/A188433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⎣2r⎦ – ⎣n r⎦ – ⎣2r – n r⎦, where r = (1+√5)/2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 138, u64 n <= 138,
 *            u128 n <= 138, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Using the identity ⎣nφ⎦ = ⎣(n + ⌊n√5⌋)/2⎦ and 2r = 1 + √5. */

#include "../utils/floor_sqrt.h"        /* Provides floor_sqrt(seq_t) */

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
