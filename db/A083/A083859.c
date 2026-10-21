/*
 * A083859  Main diagonal of generalized Fibonacci array A083856.
 * https://oeis.org/A083859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{s = 0..floor((n-1)/2)} binomial(n-1-s, s) * n^s
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 26, u128 n <= 45,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    seq_t result = 0;
    seq_t power  = 1;           /* n^0 starts at 1 */
    seq_t ns     = (seq_t)n;    /* cast once to seq_t */

    idx_t limit = (n - 1) / 2;

    for (idx_t s = 0; s <= limit; ++s)
    {
        seq_t term = binomial(n - 1 - s, s); /* binomial(n-1-s, s) */
        term *= power;                       /* multiply by n^s */
        result += term;
        power *= ns;                         /* next power of n */
    }
    return result;
}
