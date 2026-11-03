/*
 * A030505  Graham-Sloane-type lower bound on the size of a ternary (n,3,5)
 *          constant-weight code.
 * https://oeis.org/A030505
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(binomial(n,5) * 32 / (2*n + 1))
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 44 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently a(n) = ceil(binomial(n,5) * 2^5 / (2*n + 1)))  */

#include <stdlib.h>
#include "../utils/binomial.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    /*  Sequence starts at a(5); values for n < 5 are ignored. */
    if (n < 5)
        return 0;

    /* Binomial coefficient C(n,5) */
    seq_t bin = binomial(n, 5);

    /* numerator = C(n,5) * 32 = C(n,5) * 2^5 */
    seq_t num = mul_small(bin, 32);

    /* denominator = 2*n + 1 */
    seq_t denom = (seq_t)(2 * n + 1);

    /* Compute ceil division num/denom */
    seq_t q = num / denom;
    seq_t r = num % denom;
    if (r != (seq_t)0)
        ++q;

    return q;
}
