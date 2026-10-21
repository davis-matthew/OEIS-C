/*
 * A051583  a(n) = (2*n+9)!!/9!!, related to A001147 (odd double factorials).
 * https://oeis.org/A051583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+9)!! / 9!!, * a(0) = 1, * a(n+1) = (2*n + 11) * a(n)
 *            * This implementation computes the term directly from the
 *            recurrence, * avoiding any need for dynamic allocation that
 *            would clash with * arbitrary‑precision scalar types. It works
 *            for any idx_t (uint64_t) * and any seq_t (unsigned i...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t, used by seq_t constructors */

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return (seq_t)1;
    }

    /* start with the base value 1 and multiply the factors
     * 11, 13, 15, ..., (2*n + 9)  (n factors in total).
     */
    seq_t result = (seq_t)1;
    for (idx_t i = 0ull; i < n; ++i) {
        /* factor = 11 + 2*i  (fits in seq_t for all i <= 2^64-1) */
        seq_t factor = (seq_t)(11 + (seq_t)((idx_t)2 * i));
        result = result * factor;
    }
    return result;
}
