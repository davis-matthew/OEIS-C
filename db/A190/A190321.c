/*
 * A190321  Number of nonzero digits when writing n in base where place values
 *          are squares, cf. A007961.
 * https://oeis.org/A190321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of nonzero digits in the greedy representation of
 *            n * using place values k² (k = 1,2,3,…) – i.e. a(n) = |{k : ⌊n /
 *            k²⌋ ≠ 0 after * subtracting higher‑order terms}|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of an unsigned 64‑bit value, implemented without
 * floating point operations and without overflow.
 * Returns ⌊√x⌋. */
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t result = 0;
    /* The largest power of four that fits in 64 bits. */
    idx_t bit = 1ULL << 62;
    while (bit > x)
        bit >>= 2;
    while (bit) {
        if (x >= result + bit) {
            x -= result + bit;
            result = (result >> 1) + bit;
        } else {
            result >>= 1;
        }
        bit >>= 2;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t remaining = n;
    seq_t count = 0;

    /* Highest possible square index for the current remaining value. */
    idx_t k = _isqrt_u64(remaining);

    for (; k > 0; --k) {
        idx_t sq = k * k;          /* k² fits in unsigned 64‑bit. */
        idx_t digit = remaining / sq;

        if (digit != 0) {
            ++count;
            remaining -= digit * sq;
        }

        if (remaining == 0)
            break;                 /* All digits processed. */
    }

    return count;
}
