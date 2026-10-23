/*
 * A042796  Numerators of continued fraction convergents to sqrt(929).
 * https://oeis.org/A042796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numerators of continued‑fraction convergents to √929
 *
 *  a(0) = 30
 *  For n ≥ 1, let aₙ be the (n‑th) partial quotient of the simple
 *  continued fraction expansion of √929 (periodic after a₀).  Then
 *
 *      p₍ₙ₋₂₎ = 0,   p₍ₙ₋₁₎ = 1
 *      pₖ    = aₖ · p₍ₖ₋₁₎ + p₍ₖ₋₂₎        (k ≥ 0)
 *
 *  The required sequence is a(n) = pₙ.
 */

#include <stdbool.h>

/* Return the n‑th term of the sequence */
seq_t a(idx_t n) {
    /* ---------------------------------------------------------- */
    /*  Periodic part of the continued fraction for √929         */
    /* ---------------------------------------------------------- */
    static bool initialized = false;
    static seq_t period[64];
    static idx_t period_len;
    static const seq_t a0 = (seq_t)30;          /* floor(sqrt(929)) */

    if (!initialized) {
        /* Use 64‑bit arithmetic for the period generation;
           all intermediate values of the algorithm are ≤ 929.   */
        uint64_t m   = 0;
        uint64_t d   = 1;
        uint64_t a    = 30;                     /* a₀ */
        idx_t    i   = 0;

        do {
            m = d * a - m;
            d = ((uint64_t)929 - m * m) / d;
            a = (30 + m) / d;
            period[i++] = (seq_t)a;             /* a₁, a₂, … , 2·a₀ … */
        } while (a != 60);                       /* stop at 2*a₀ */

        period_len = i;
        initialized = true;
    }

    /* ---------------------------------------------------------- */
    /*  Recurrence for numerators                               */
    /* ---------------------------------------------------------- */
    seq_t p_prev2 = (seq_t)0;   /* p_{k-2}   */
    seq_t p_prev1 = (seq_t)1;   /* p_{k-1}   */
    seq_t p_next  = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t coeff;
        if (k == 0)          /* a₀ */
            coeff = a0;
        else
            coeff = period[(k - 1) % period_len];
        p_next  = coeff * p_prev1 + p_prev2;
        p_prev2 = p_prev1;
        p_prev1 = p_next;
    }
    return p_prev1;
}
