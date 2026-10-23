/*
 * A041358  Numerators of continued fraction convergents to sqrt(193).
 * https://oeis.org/A041358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(193).
   Recurrence:
     a(0) = 13, a(1) = 14
     For n ≥ 2:
         a(n) = q_n * a(n‑1) + a(n‑2)
   where the partial quotients q_n of √193 are:
         q_0 = 13, and for n ≥ 1
         q_n = period[(n‑1) % 13] with
         period = {1, 8, 3, 2, 1, 3, 3, 1, 2, 3, 8, 1, 26}.
   This computes each term in O(n) time. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Periodic part of the continued fraction of √193. */
    static const uint8_t period[13] = {1, 8, 3, 2, 1, 3, 3, 1, 2, 3, 8, 1, 26};
    const size_t R = sizeof period / sizeof period[0];

    if (n == 0) return 13;
    if (n == 1) return 14;

    seq_t p_prevprev = 13;   /* a(0) */
    seq_t p_prev     = 14;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        size_t idx = (size_t)(i - 1) % R;        /* fetch q_i */
        seq_t q    = (seq_t)period[idx];
        seq_t p_next = q * p_prev + p_prevprev;  /* recurrence */
        p_prevprev = p_prev;
        p_prev     = p_next;
    }
    return p_prev;
}
