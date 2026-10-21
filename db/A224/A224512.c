/*
 * A224512  Gray code variant of A147582.
 * https://oeis.org/A224512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * 3^(gt(n‑1)-1), where gt(k) is the Gray code weight of
 *            k
 * Range:     u8 n <= 21, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A224512, offset 1. */

#include <stdint.h>

/* Helper: popcount for 64‑bit unsigned values. */
static unsigned popcount_u64(idx_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - 1; /* clear least significant bit set */
        ++c;
    }
    return c;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* not part of the OEIS offset, but safe */
    if (n == 1) return 1;      /* a(1) = 1 */

    idx_t m = n - 1;                 /* index for Gray code */
    idx_t gray = m ^ (m >> 1);       /* binary reflected Gray code of m */
    unsigned weight = popcount_u64(gray);  /* number of 1‑bits in gray */

    /* weight is at least 1 for all m >= 1 */
    seq_t result = 1;
    unsigned exp = weight - 1;       /* exponent for 3 */
    while (exp--) {
        result *= (seq_t)3;
    }
    result *= (seq_t)4;              /* multiply by 4 */

    return result;
}
