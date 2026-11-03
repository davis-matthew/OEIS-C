/*
 * A048723  Binary "exponentiation" without carries: {0..y}^{0..x}, where y
 *          (column index) is binary encoding of GF(2)-polynomial and x (row
 *          index) is the exponent.
 * https://oeis.org/A048723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 81, u32 n <= 197, u64 n <= 472,
 *            u128 n <= 1191, big any n
 * Validated: standard, pass, 82 terms through n = 9875 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A048723:  a(n) = x (⊗) y  where  ⊗ is carry‑less
   (no‑carry) multiplication and (x,y) are the Cantor pairing coordinates
   of n (row index, column index). */
#include <stdint.h>

/* Carry‑less (no‑carry) multiplication: a ⊗ b. */
static inline seq_t carryless_mult(seq_t a, seq_t b)
{
    seq_t res = 0;
    while (a) {
        if (a & 1)
            res ^= b;
        a >>= 1;
        b <<= 1;
    }
    return res;
}

/* Carry‑less exponentiation: base ^ exp. */
static seq_t carryless_pow(seq_t base, seq_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1)
            result = carryless_mult(result, base);
        base = carryless_mult(base, base);
        exp >>= 1;
    }
    return result;
}

/* Integer square root for unsigned 128‑bit values. */
static inline idx_t isqrt_u128(unsigned __int128 v)
{
    idx_t low = 0;
    /* 8*n+1 fits in 128 bits; its square root is < 2^35. */
    idx_t high = 1ULL << 35;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* ceil(mid) */
        if (((unsigned __int128)mid * mid) <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Triangular inverse: t = floor((sqrt(8*n + 1) - 1) / 2) */
    idx_t t = (isqrt_u128((unsigned __int128)8 * n + 1ULL) - 1) / 2;

    /* Row index x = n - t*(t+1)/2  */
    idx_t tri = (unsigned __int128)t * (t + 1) / 2;
    idx_t x = n - tri;

    /* Column index y = t - x  */
    idx_t y = t - x;

    return carryless_pow((seq_t)x, (seq_t)y);
}
