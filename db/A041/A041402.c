/*
 * A041402  Numerators of continued fraction convergents to sqrt(216).
 * https://oeis.org/A041402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(216)
   The continued fraction of sqrt(216) is
       [14; 1, 2, 3, 2, 1, 28, 1, 2, 3, 2, 1, 28, ...]
   i.e. a_0 = 14 and for i ≥ 1 the partial quotients repeat the
   block [1, 2, 3, 2, 1, 28].
   The numerators p_i of the convergents satisfy
       p_{-2} = 0,  p_{-1} = 1,
       p_i   = a_i * p_{i-1} + p_{i-2}   for i ≥ 0.
   This function returns p_n, i.e. a(n) for the OEIS sequence A041402. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for the very first term */
    if (n == 0)
        return (seq_t)14;

    /* State for the linear recurrence */
    seq_t p_prev2 = (seq_t)0;                  /* p_{-2} */
    seq_t p_prev1 = (seq_t)1;                  /* p_{-1} */
    seq_t p_curr;                              /* current numerator */

    /* Iterate over the positions of the partial quotients */
    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t ai;
        if (i == 0) {
            ai = (seq_t)14;
        } else {
            /* i-1 modulo 6 gives the position inside the repeating block */
            uint64_t r = (uint64_t)(i - 1) % 6ULL;
            switch (r) {
                case 0: ai = (seq_t)1; break;
                case 1: ai = (seq_t)2; break;
                case 2: ai = (seq_t)3; break;
                case 3: ai = (seq_t)2; break;
                case 4: ai = (seq_t)1; break;
                case 5: ai = (seq_t)28; break;
                default: ai = (seq_t)0; /* never reached */
            }
        }

        /* Compute the next numerator using the recurrence */
        p_curr = ai * p_prev1 + p_prev2;

        /* Shift the state for the next iteration */
        p_prev2 = p_prev1;
        p_prev1 = p_curr;
    }

    return p_curr;
}
