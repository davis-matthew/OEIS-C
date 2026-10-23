/*
 * A137353  Minimal number of edges in a (P_4,n)-edge stable graph.
 * https://oeis.org/A137353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ceil( (sqrt(8*n + 9) + 3) / 2 ), for n >= 2. a(1) = 4
 * Range:     u8 n <= 6, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"    /* floor_sqrt returns a seq_t */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined offset; return 0 safety */
    if (n == 1) return (seq_t)4;          /* base term */
    seq_t x = ((seq_t)8 * (seq_t)n) + (seq_t)9;
    seq_t t = floor_sqrt(x);              /* integer floor of sqrt(8*n+9) */
    seq_t ceil_term;
    if (t * t == x)                      /* 8*n+9 is a perfect square */
        ceil_term = (t + (seq_t)4) / (seq_t)2;
    else                                  /* otherwise */
        ceil_term = (t + (seq_t)5) / (seq_t)2;
    return (seq_t)n + ceil_term;
}
