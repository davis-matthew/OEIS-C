/*
 * A099033  a(n) = Sum_{k=1..n} (-1)^A000120(3*k).
 * https://oeis.org/A099033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 707, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Σ_{k=1..n} (-1)^{A000120(3k)}
 *          where A000120(m) = number of 1's in the binary expansion of m.
 *          So each term is +1 if popcount(3k) is even, -1 if odd.
 *
 * This implementation uses an efficient parity test on a 64‑bit value.
 * The function runs in O(n) time and uses only integer arithmetic.
 */

#include <stdint.h>

static inline unsigned int popcount_parity_uint64(uint64_t x)
{
    /* compute parity (popcount mod 2) without loops */
    x ^= x >> 32;
    x ^= x >> 16;
    x ^= x >> 8;
    x ^= x >> 4;
    x ^= x >> 2;
    x ^= x >> 1;
    return (unsigned int)(x & 1u);
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* compute 3k incrementally to avoid overflow in the multiplication */
    uint64_t value = 3ULL;          /* 3*1 */
    for (idx_t k = 1; k <= n; ++k) {
        /* parity of popcount of value */
        unsigned int p = popcount_parity_uint64(value);
        if (p) {
            /* odd popcount → -1 */
            sum--;
        } else {
            /* even popcount → +1 */
            sum++;
        }
        value += 3ULL;              /* next 3*(k+1) */
    }
    return sum;
}
