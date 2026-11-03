/*
 * A083495  Multiples of 7 in which there is no common digit in successive
 *          terms.
 * https://oeis.org/A083495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: successive multiples of 7 that share no common digit with the previous term */
#include <stdbool.h>

/* Return true if a and b share any decimal digit */
static inline bool common_digits(seq_t a, seq_t b)
{
    unsigned maskA = 0, maskB = 0;
    seq_t t = a;
    while (t > 0) {
        maskA |= 1u << (unsigned)(t % 10);
        t /= 10;
    }
    t = b;
    while (t > 0) {
        maskB |= 1u << (unsigned)(t % 10);
        t /= 10;
    }
    return (maskA & maskB) != 0;
}

seq_t a(idx_t n)
{
    seq_t current = 7;          /* a(0) */
    if (n == 0) return current;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t candidate = current + 7;      /* next multiple of 7 */
        while (common_digits(current, candidate))
            candidate += 7;                 /* skip until digits disjoint */
        current = candidate;
    }
    return current;
}
