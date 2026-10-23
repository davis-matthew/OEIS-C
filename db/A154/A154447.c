/*
 * A154447  Permutation of nonnegative integers induced by wreath recursion
 *          a=s(b,c), b=s(c,a), c=(c,c), starting from state b, rewriting bits
 *          from the second most significant bit toward the least significant
 *          end.
 * https://oeis.org/A154447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 2047, u32 n <= 2047, u64 n <= 2047,
 *            u128 n <= 2047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A154447: a permutation of the natural numbers induced by
 * the wreath recursion a=s(b,c), b=s(c,a), c=(c,c). For a non‑negative
 * integer n the binary expansion is scanned rightward from the second
 * most significant bit; the first bit that satisfies
 *
 *   (bit == 0 and the distance from the msb is odd)   or
 *   (bit == 1 and the distance from the msb is even)
 *
 * stops the scan. All bits from the most significant bit down to this
 * stopping position (inclusive) are complemented. The resulting integer
 * is a(n).  This implementation works for any seq_t type supported by
 * the build (uint8_t … uint128_t or arbitrary precision) and does not
 * depend on external helpers such as bitlen_seq_t.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) = 0 */
        return 0;

    seq_t sn = (seq_t)n; /* n fits into seq_t for all tested values */

    /* Find position of the most significant set bit (msb). */
    seq_t tmp = sn;
    unsigned m = 0;      /* msb index, 0‑based */
    while ((tmp >> 1) != 0) {
        tmp >>= 1;
        ++m;
    }

    if (m == 0)          /* n == 1, only one bit */
        return sn;

    /* Scan bits from msb‑1 down to 0 to find the stopping position. */
    int64_t stop = -1;   /* -1 means no stop found */
    for (int64_t pos = (int64_t)m - 1; pos >= 0; --pos) {
        seq_t bit   = (sn >> (unsigned)pos) & (seq_t)1;
        unsigned dist = m - (unsigned)pos;   /* distance from msb */

        bool cond = (bit == 0 && (dist & 1u))   /* zero‑bit at odd distance */
                  || (bit == 1 && !(dist & 1u)); /* one‑bit at even distance */

        if (cond) {
            stop = pos;
            break;
        }
    }

    /* Build the mask of bits to complement. */
    seq_t mask;
    if (stop == -1) {
        /* No stopping bit: complement all lower bits. */
        mask = (((seq_t)1 << m) - 1);
    } else {
        unsigned len = m - (unsigned)stop;      /* number of bits to flip */
        mask = (((seq_t)1 << len) - 1) << (unsigned)stop;
    }

    return sn ^ mask;      /* complement the selected bits */
}
