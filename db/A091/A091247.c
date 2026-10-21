/*
 * A091247  Characteristic function of A091242: 1 if the n-th GF(2)[X]
 *          polynomial is reducible, 0 otherwise.
 * https://oeis.org/A091247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9280, u16 n <= 9280, u32 n <= 9280, u64 n <= 9280,
 *            u128 n <= 9280, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A091247: Characteristic function of A091242.
 * Returns 1 if the n-th polynomial over GF(2)[X] (starting with the zero
 * polynomial) is reducible, 0 otherwise.
 *
 * The n-th polynomial is obtained by listing all polynomials over GF(2)
 * in lexicographic order of increasing degree:
 *   index 0   -> 0
 *   index 1   -> 1
 *   index 2   -> x
 *   index 3   -> x+1
 *   index 4   -> x^2
 *   index 5   -> x^2+1
 *   ...
 * The polynomial at index n (n ≥ 1) has degree d = floor(log2(n)),
 * and its lower‑degree coefficients are given by the bits of n−2^d.
 *
 * The function checks reducibility by exhaustive trial division using
 * all monic divisor polynomials of degree 1 … d/2.
 * All work is performed in 64‑bit integer arithmetic as the maximum
 * degree for n ≤ 10000 is less than 14.
 */

#include <stdint.h>

static int deg_uint64(uint64_t x)
{
    int d = -1;
    while (x) {
        ++d;
        x >>= 1;
    }
    return d;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* zero polynomial is not considered reducible */
        return (seq_t)0;

    /* find degree d = floor(log2(n)) */
    uint64_t nn = (uint64_t)n;
    unsigned d = 0;
    while ((uint64_t)1 << (d + 1) <= nn)
        ++d;

    /* polynomial bit pattern: leading 1 at degree d, lower bits from offset */
    uint64_t offset = nn - ((uint64_t)1 << d);      /* offset < 2^d */
    uint64_t poly   = ((uint64_t)1 << d) | offset;   /* poly fits in <64 bits */

    /* constant 1 and linear polynomials are never reducible */
    if (d <= 1)
        return (seq_t)0;

    /* trial division by all monic polynomials of degree 1 … d/2 */
    for (unsigned t = 1; t <= d / 2; ++t) {
        uint64_t qbase = (uint64_t)1 << t;
        for (uint64_t m = 0; m < qbase; ++m) {
            uint64_t q = qbase | m;      /* monic divisor of degree t */

            uint64_t rem = poly;
            /* polynomial division (modulus) in GF(2) */
            while (rem) {
                int deg_rem = deg_uint64(rem);
                if (deg_rem < (int)t)
                    break;
                unsigned shift = deg_rem - t;
                rem ^= q << shift;
            }
            if (rem == 0)
                return (seq_t)1;          /* reducible */
        }
    }
    return (seq_t)0;                       /* irreducible */
}
