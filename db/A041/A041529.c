/*
 * A041529  Denominators of continued fraction convergents to sqrt(281).
 * https://oeis.org/A041529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 21, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Denominators of continued‑fraction convergents to √281.
   Recurrence:
       a(0) = 1
       a(1) = 1
       a(n) = c_n * a(n-1) + a(n-2)  for n >= 2
   where the coefficient c_n is
       c_0 = 16
       for n > 0,
           c_n = period[(n-1) % 13]
   and the period of the continued fraction of √281 is
       {1, 3, 4, 1, 1, 6, 6, 1, 1, 4, 3, 1, 32}. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    static const seq_t period[13] = {
        (seq_t)1, (seq_t)3, (seq_t)4, (seq_t)1, (seq_t)1,
        (seq_t)6, (seq_t)6, (seq_t)1, (seq_t)1, (seq_t)4,
        (seq_t)3, (seq_t)1, (seq_t)32
    };

    seq_t prev2 = (seq_t)1;   /* Q_{-2} */
    seq_t prev1 = (seq_t)0;   /* Q_{-1} */

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        seq_t coeff;
        if (k == (idx_t)0)
            coeff = (seq_t)16;
        else
            coeff = period[(size_t)((k - (idx_t)1) % 13)];

        seq_t next = coeff * prev1 + prev2;
        prev2 = prev1;
        prev1 = next;
    }

    return prev1;
}
