/*
 * A057268  a(n-1)+k=a(n) => a(n)*k=a(n+1).
 * https://oeis.org/A057268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 3, for n >= 2, a(n) = a(n-1)*(a(n-1)-a(n-2))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)3;

    /* Compute iteratively; only need the two last terms. */
    seq_t prev_prev = (seq_t)1;  /* a(0) */
    seq_t prev      = (seq_t)3;  /* a(1) */
    seq_t next;
    for (idx_t i = 2; i <= n; ++i) {
        next = prev * (prev - prev_prev);
        prev_prev = prev;
        prev = next;
    }
    return prev;
}
