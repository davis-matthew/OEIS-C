/*
 * A160663  Number of distinct sums that one can obtain by adding two squares
 *          among the n first ones.
 * https://oeis.org/A160663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct values in the multiset {k^2 : 1 ≤ k ≤
 *            n} ∪ {k^2 + l^2 : 1 ≤ k,l ≤ n}
 * Range:     u8 n <= 23, u16 n <= 453, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a bitset up to 2 n² to count distinct sums. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* bitset size: 0 … 2*n*n inclusive */
    const uint64_t max_sum = (uint64_t)2 * (uint64_t)n * (uint64_t)n;
    const size_t bits = (size_t)(max_sum + 1);
    const size_t bytes = (bits + 7) >> 3;
    uint8_t *const bit = (uint8_t *)malloc(bytes);
    if (!bit) return 0;  /* allocation failed; return 0 as safe fallback */

    for (size_t i = 0; i < bytes; ++i) bit[i] = 0;

    seq_t count = 0;

    /* Helper macro to set a bit and increase count if it was not set */
#define SET_BIT(v)                                 \
    do {                                           \
        const size_t byte = ((size_t)(v) >> 3);    \
        const uint8_t mask = (uint8_t)(1 << ((size_t)(v) & 7)); \
        if (!(bit[byte] & mask)) {                 \
            bit[byte] |= mask;                     \
            ++count;                               \
        }                                          \
    } while (0)

    for (idx_t i = 1; i <= n; ++i) {
        const uint64_t ii = (uint64_t)i * (uint64_t)i;
        SET_BIT(ii);                  /* add the square itself */

        for (idx_t j = i; j <= n; ++j) {
            const uint64_t jj = (uint64_t)j * (uint64_t)j;
            const uint64_t s = ii + jj;          /* i^2 + j^2 */
            SET_BIT(s);
        }
    }

    free(bit);
    return count;
}
