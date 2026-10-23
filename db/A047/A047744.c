/*
 * A047744  A discrete analog of Li(n): round ( Sum_{k=2..n} 1/log_2 (k) ).
 * https://oeis.org/A047744
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round( Σ_{k=2..n} 1 / log₂(k) )
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    if (n < 2)
        return (seq_t)0;                    /* undefined for n<2, return 0 */

    double sum = 0.0;
    for (idx_t k = 2; k <= n; ++k)
        sum += 1.0 / log2((double)k);

    return (seq_t)llround(sum);              /* round to nearest integer  */
}
