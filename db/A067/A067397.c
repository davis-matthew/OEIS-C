/*
 * A067397  Maximal power of 3 that divides n-th Catalan number.
 * https://oeis.org/A067397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9740 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A067397 – Maximal power of 3 that divides the n‑th Catalan number.
 * Recurrence (for n ≥ 0):
 *   a(0) = 0.  Let k = ⌊log₃ n⌋,  base = 3ᵏ.
 *   If n == 3·base – 1             → a(n) = 0.
 *   Else if  (3/2)·base < n < (5/2)·base
 *        → a(n) = a(n – base) + 1.
 *   Else                            → a(n) = a(n – base).
 */

#include <stdint.h>

static seq_t a_rec(idx_t n)
{
    if (n == 0) {                   /* base case */
        return 0;
    }

    seq_t n_s   = (seq_t)n;         /* cast index to seq_t for arithmetic */

    /* Find the largest power of 3 ≤ n.  Since n ≤ 2^64‑1 this
       fits into seq_t and all intermediate values stay within the
       range of an arbitrary‑precision implementation. */
    seq_t base = 1;
    for (;;) {
        seq_t next = base * 3;
        if (next > n_s) break;
        base = next;
    }

    /* Recursive evaluation for the reduced index.  base ≤ n, so the
       subtraction fits into idx_t. */
    idx_t sub_n = n - (idx_t)base;
    seq_t sub   = a_rec(sub_n);

    /* Special zero case: n = 3*base – 1. */
    if (n_s == base * 3 - 1)
        return 0;

    /* Check the interval (3/2)*base < n < (5/2)*base.
       Multiply by 2 to avoid fractions. */
    seq_t two_n  = n_s * 2;
    seq_t thr_lo = base * 3;   /* 3/2 * base * 2  */
    seq_t thr_hi = base * 5;   /* 5/2 * base * 2  */
    if (two_n > thr_lo && two_n < thr_hi)
        return sub + 1;

    return sub;
}

seq_t a(idx_t n)
{
    return a_rec(n);
}
