/*
 * A041626  Numerators of continued fraction convergents to sqrt(332).
 * https://oeis.org/A041626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numerators of continued fraction convergents to sqrt(332).
   Recurrence: p_{-2}=0, p_{-1}=1.
   For n>=0: p_n = a_n * p_{n-1} + p_{n-2},
   where a_0 = ⌊√332⌋ = 18 and a_k (k>0) repeats the
   period [4,1,1,8,1,1,4,36] (a_{k} = period[(k-1) mod 8]).
   Returns the nth numerator p_n. */

#include <stdlib.h>

static const seq_t period[8] = {4, 1, 1, 8, 1, 1, 4, 36};
static const seq_t a0 = 18;

seq_t a(idx_t n)
{
    seq_t prev2 = 0;           /* p_{-2} */
    seq_t prev1 = 1;           /* p_{-1} */

    for (idx_t i = 0; i <= n; ++i)
    {
        seq_t a_term = (i == 0) ? a0 : period[(i - 1) % 8];
        seq_t curr    = a_term * prev1 + prev2;  /* p_i */

        /* Shift: p_{i-2} -> p_{i-1}, p_{i-1} -> p_i */
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;  /* p_n */
}
