/*
 * A164885  Length of preperiodic part of trajectory of n under iteration of
 *          the base-2 Kaprekar map in A164884.
 * https://oeis.org/A164885
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of iterations of the base‑2 Kaprekar map until a
 *            fixed point is reached (preperiodic part length)
 * Range:     u8 n <= 1024, u16 n <= 1024, u32 n <= 1024, u64 n <= 1024,
 *            u128 n <= 1024, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline uint64_t kaprekar_next(uint64_t x) {
    if (x == 0) return 0;
    uint64_t L = 64 - __builtin_clzll(x);          /* bit length of x */
    uint64_t c = __builtin_popcountll(x);          /* number of 1‑bits */
    uint64_t max, min;
    if (c == 0) {                                 /* only for x==0, handled above */
        max = min = 0;
    } else if (c == 64) {                         /* all bits are 1 */
        max = ~0ULL;
        min = ~0ULL;
    } else {
        max = ((1ULL << c) - 1ULL) << (L - c);
        min = (1ULL << c) - 1ULL;
    }
    return max - min;
}

seq_t a(idx_t n) {
    uint64_t x = (uint64_t)n;          /* loop variable, fits in 64 bits */
    seq_t steps = 0;
    for (;;) {
        if (x == 0) break;            /* already at fixed point 0 */
        uint64_t next = kaprekar_next(x);
        if (next == x) break;         /* x is a non‑zero fixed point */
        x = next;
        steps++;
    }
    return (seq_t)steps;
}
