/*
 * A065399  a(n) is the greatest integer m such that (1) written in base n it
 *          is also readable as if it is in factorial base and (2) this
 *          factorial base value is smaller than or equal to m.
 * https://oeis.org/A065399
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum integer m such that m's base‑n representation,
 *            interpreted as a factorial base number, yields a value ≤ m. The
 *            digits d_i (i ≥ 1) satisfy 0 ≤ d_i ≤ min(i, n − 1) and m = ∑ d_i
 *            · n^(i−1), m_fact = ∑ d_i · i!. The algorithm sets digits
 *            greedily from the least significant position. Fo...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Assume n ≥ 2 (offset 2). */
    seq_t m     = 0;        /* value in base n */
    seq_t v     = 0;        /* value in factorial base */
    seq_t pow_n = 1;        /* n^(i-1) for current i */
    seq_t fact  = 1;        /* i! for current i */
    idx_t i     = 1;        /* position counter (least significant = 1) */

    for (;;) {
        /* Maximum digit allowed at position i: min(i, n-1). */
        idx_t maxdig = (i <= n-1) ? i : (n-1);

        /* Find the largest feasible digit at this position. */
        idx_t best = 0;
        for (idx_t d = maxdig; ; --d) {
            unsigned __int128 new_m = (unsigned __int128)m
                                     + (unsigned __int128)pow_n * d;
            unsigned __int128 new_v = (unsigned __int128)v
                                     + (unsigned __int128)fact * d;
            if (new_v <= new_m) {
                best = d;
                break;
            }
            if (d == 0) {         /* should never happen, keeps compiler happy */
                best = 0;
                break;
            }
        }

        /* Update the accumulated values. */
        m += pow_n * best;
        v += fact * best;

        /* If we had to use a digit smaller than the maximum, we are done. */
        if (best < maxdig)
            break;

        /* Advance to next position. */
        pow_n *= (seq_t)n;                 /* next power of n */
        fact  *= (seq_t)(i + 1);           /* next factorial */
        ++i;
    }

    return m;
}
