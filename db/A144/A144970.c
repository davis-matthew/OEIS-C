/*
 * A144970  Integers in the sequence of rationals defined by b(1) = 61, b(n) =
 *          b(n-1) + (n+60)/n for n > 1.
 * https://oeis.org/A144970
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   a(n) = 61 + Σ_{k=2..n} (k+60)/k for n ≤ 6; for n > 6 the term is
 *            not an integer, return 0
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* For n == 0 (below offset) return 0 */
    if (n == 0) return 0;
    seq_t result = 61;   /* b(1) = 61 */
    for (idx_t i = 2; i <= n && i <= 6; ++i) {
        /* (i + 60) / i is integer for i = 2..6 */
        seq_t inc = (seq_t)(i + 60) / (seq_t)i;
        result += inc;
    }
    /* If n > 6, the sequence has no integer terms – return 0 */
    if (n > 6) return 0;
    return result;
}
