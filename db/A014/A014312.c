/*
 * A014312  Numbers with exactly 4 ones in binary expansion.
 * https://oeis.org/A014312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers having exactly 4 ones in their binary expansion. Uses a
 *            simple linear search: iterate numbers, count 1‑bits until the
 *            desired rank is reached. This works for any n ≥ 1 and is fast
 *            enough for n up to about 10000 (the 10000th term is < 2²⁴)
 * Range:     u8 n <= 70, u16 n <= 1820, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline unsigned popcount64(unsigned long long x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    /* Portable bit‑twiddling popcount for 64‑bit integers. */
    x -= (x >> 1) & 0x5555555555555555ULL;
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    return (unsigned)((x * 0x0101010101010101ULL) >> 56);
#endif
}

seq_t a(idx_t n)
{
    if (n == 0)           /* not part of the sequence, but defined for safety */
        return 0;

    unsigned long long count = 0;
    unsigned long long x = 15;           /* first number with 4 ones: 1111₂ */

    while (1) {
        if (popcount64(x) == 4) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
        ++x;
    }
}
