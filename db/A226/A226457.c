/*
 * A226457  D(n,2^n), where D is the binary graph metric, as in A226456.
 * https://oeis.org/A226457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = D(n, 2^n) where D is the binary graph metric between n
 *            and 2^n—i.e., the length of the shortest path in the infinite
 *            binary tree with edges x ↔ 2x and x ↔ 2x+1. For any n ≥ 1: h =
 *            ⌊log₂(n)⌋ (depth of n) if n is a power of two: a(n) = n - h
 *            else: r = ⌊log₂(n XOR 2^h)⌋ (position of second‑mo...
 * Range:     u8 n <= 248, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log₂(x)) for x > 0.  Uses only integer shifts. */
static idx_t floor_log2_idx(idx_t x)
{
    idx_t r = 0;
    while (x >>= 1)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)                         /* offset is 1; n=0 is unused */
        return (seq_t)0;

    /* h = ⌊log₂(n)⌋ */
    idx_t h = floor_log2_idx(n);

    /* mask has bit h set */
    idx_t mask = ((idx_t)1 << h);

    if (n == mask)                     /* n is a power of two */
    {
        return (seq_t)(n - h);
    }

    /* m = n XOR mask clears the highest set bit, leaving lower bits */
    idx_t m = n ^ mask;

    /* r = ⌊log₂(m)⌋ gives the position of the next lower 1-bit */
    idx_t r = floor_log2_idx(m);

    return (seq_t)(n - h + 2 * (r + 1));
}
