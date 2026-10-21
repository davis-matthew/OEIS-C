/*
 * A061549  Denominator of probability that there is no error when average of
 *          2n numbers is computed, assuming errors of +1, -1 are possible and
 *          they each occur with p=1/4.
 * https://oeis.org/A061549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, changed
 * Formula:   a(n) = 2^( 4*n – popcount(n) ) (n ≥ 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  popcount(n) – number of 1‑bits in the binary representation of n  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

static inline uint64_t popcount_uint64(uint64_t x) {
    /* Built‑in/popcount loop fallback (compiler may provide __builtin_popcountll) */
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    uint64_t c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

seq_t a(idx_t n)
{
    /* n is uint64_t, safe to cast for bit‑counting */
    uint64_t bits = popcount_uint64((uint64_t)n);
    /* exponent e = 4*n – popcount(n)  */
    uint64_t e = (uint64_t)4 * n - bits;

    /* For 64‑bit seq_t use a shift if the exponent fits a single shift.
       For larger seq_t or large exponent use big_pow (suitable for arbitrary‑precision).  */
    if (sizeof(seq_t) <= 8 && e < 64)
        return ((seq_t)1) << e;

    /* For seq_t that cannot hold the result directly, or exponent too large,
       compute 2^e via the big integer routine. */
    return big_pow((seq_t)2, (seq_t)e);
}
