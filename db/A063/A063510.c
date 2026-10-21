/*
 * A063510  a(1) = 1, a(n) = a(floor(square root(n))) + 1 for n > 1.
 * https://oeis.org/A063510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1; for n>1: a(n) = a(floor(sqrt(n))) + 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for idx_t (unsigned 64‑bit).  Returns floor(sqrt(n)). */
static inline idx_t int_sqrt(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = n >> 1;
    idx_t result = 1;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t square = mid * mid;   /* fits in 64 bits for idx_t max value */
        if (square == n) return mid;
        if (square < n) {
            result = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;   /* safety, should never happen */
            high = mid - 1;
        }
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined part of sequence, safe guard */
    seq_t value = 1;               /* a(1) = 1 */
    idx_t x = n;
    while (x > 1) {                /* iterate recurrence */
        x = int_sqrt(x);
        value++;
    }
    return value;
}
