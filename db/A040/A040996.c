/*
 * A040996  Maximum number of distinct functions at the bottom of a Boolean
 *          (or Binary) Decision Diagram (or BDD) with negation by pointer
 *          complementation.
 * https://oeis.org/A040996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (S(n‑1) + 1) * (2*S(n‑1) + 1) where S(n‑1) = sum_{k < n}
 *            a(k). This equality generates the OEIS sequence A040996
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;           /* S(k-1) at the start of each loop */
    seq_t term = 0;
    const seq_t one = 1;
    const seq_t two = 2;

    for (idx_t i = 0; i <= n; ++i) {
        /* term = (sum + 1) * (2*sum + 1) */
        term = (sum + one) * ((sum * two) + one);
        if (i == n)
            return term;
        sum += term;        /* update the cumulative sum */
    }
    return term;            /* never reached, but keeps the compiler happy */
}
