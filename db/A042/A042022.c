/*
 * A042022  Numerators of continued fraction convergents to sqrt(535).
 * https://oeis.org/A042022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 15, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A042022: Numerators of continued‑fraction convergents to √535.
 *  The sequence is obtained by the standard recurrence for continued
 *  fraction convergents. For each n we compute the n-th partial quotient a_n
 *  of √535 and then update the numerator with
 *
 *      p_{-2} = 0 ,  p_{-1} = 1
 *      p_n   = a_n * p_{n-1} + p_{n-2}
 *
 *  where the partial quotients start with a_0 = ⌊√535⌋ = 23, followed by the
 *  periodic sequence (7,1,2,4,1,3,1,4,2,1,7,46).  The function returns
 *  p_n for any n ≥ 0.  The algorithm runs in O(n) time and uses only
 *  integer arithmetic; all intermediate values fit comfortably in 64‑bit
 *  unsigned integers.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Constant definition of D = 535 and its integer part a0 = 23. */
    const uint64_t D   = 535ULL;
    const uint64_t a0  = 23ULL;

    /* Variables for the continued fraction algorithm: m, d, a */
    uint64_t m   = 0ULL;
    uint64_t d   = 1ULL;
    uint64_t an  = a0;  /* current partial quotient a_n */

    /* Recurrence for the convergent numerators */
    seq_t p_m2 = 0;   /* p_{-2} */
    seq_t p_m1 = 1;   /* p_{-1} */
    seq_t p_cur;

    /* Compute successive convergents up to index n. */
    for (idx_t i = 0; i <= n; ++i) {
        /* current numerator p_i */
        p_cur = (seq_t)an * p_m1 + p_m2;

        /* shift for next iteration */
        p_m2 = p_m1;
        p_m1 = p_cur;

        /* If we've reached the desired index, stop.  Skip computing
         * the next partial quotient. */
        if (i == n) break;

        /* Update m, d, a for the next partial quotient. */
        m  = d * an - m;
        d  = (D - m * m) / d;
        an = (a0 + m) / d;
    }

    /* p_m1 now holds p_n, the desired numerator. */
    return p_m1;
}
