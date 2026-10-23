/*
 * A026316  Numbers k such that |sin(k)| > |cos(k+1)|.
 * https://oeis.org/A026316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A026316
   Non-recursive brute‑force search: k such that |sin(k)| > |cos(k+1)|.
   Uses double precision to evaluate sin and cos; this is sufficient
   for the required range (n ≤ 10 000) because the probability of
   near‑ties is negligible.                                       */
#include <math.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Reject invalid indices (offset is 1). */
    if (n == 0) return (seq_t)0;

    idx_t k = 1;
    idx_t found = 0;

    for (;; ++k)
    {
        double s = sin((double)k);
        double c = cos((double)(k + 1));
        if (fabs(s) > fabs(c))
        {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
    }
}
