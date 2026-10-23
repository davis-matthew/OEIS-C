/*
 * A032935  Numbers whose set of base-15 digits is {1,2}.
 * https://oeis.org/A032935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest positive integer whose base‑15 digits are only 1
 *            or 2. For the first n terms, treat n‑1 as a binary number of
 *            length k = ⌈log₂(n+2)⌉‑1, map 0 → 1 and 1 → 2, and interpret the
 *            result as a base‑15 number. This generates the sequence in
 *            sorted order
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 766, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are defined by the build environment. */
seq_t a(idx_t n)
{
    /* Find the length k of the base‑15 representation for the nth term. */
    idx_t k = 1;          /* current digit length */
    idx_t pow2 = (idx_t)2;/* number of terms with current length */
    idx_t count = 0;      /* number of terms of lengths < k */

    /* Keep adding lengths while we have not reached term n. */
    while (count + pow2 < n) {
        count += pow2;          /* skip this block of length k */
        if (pow2 > ((~(idx_t)0) >> 1)) break; /* avoid shift overflow */
        pow2 <<= 1;            /* next block has double size */
        ++k;
    }

    /* Index of the term within its block (0‑based). */
    idx_t m = n - count - 1;

    /* Convert that index to the corresponding number. */
    seq_t result = 0;
    for (idx_t i = k; i-- > 0; ) {
        result = result * (seq_t)15 + (seq_t)(1 + ((m >> i) & 1));
    }
    return result;
}
