/*
 * A105512  Number of times 2 is the leading digit of the first n+1 Fibonacci
 *          numbers in decimal representation.
 * https://oeis.org/A105512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{k : 0 ≤ k ≤ n and the leading decimal digit of the
 *            Fibonacci number F(k) is 2}. The leading digit is obtained via
 *            the logarithmic formula log10(F(k)) ≈ k·log10(φ) – log10(√5)
 *            with a fixed‑point arithmetic to avoid floating‑point errors.
 *            For k ≤ 7 the approximation is insufficient, so the...
 * Range:     u8 n <= 1448, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer types provided by the build:
   typedef ... idx_t;   // e.g., uint64_t
   typedef ... seq_t;   // e.g., uint32_t / uint64_t / unsigned __int128
*/

static const uint64_t SCALE            = 1000000000000ULL;          /* 10^12  fixed point scale */
static const uint64_t LOG10_PHI_SCALED = 208987640249ULL;          /* floor(log10(φ)·SCALE)   */
static const uint64_t LOG10_SQRT5_SCALED = 349485002168ULL;        /* floor(log10(√5)·SCALE) */
static const uint64_t TH2_LOWER = 301029995663ULL;                 /* floor(log10(2)·SCALE)   */
static const uint64_t TH2_UPPER = 477121254719ULL;                 /* floor(log10(3)·SCALE)   */

/* Small table of leading digits for the first eight Fibonacci numbers */
static const int LD_SMALL[8] = {0, 1, 1, 2, 3, 5, 8, 1};

/* Return 1 if the leading digit of F(k) is 2, otherwise 0 */
static inline int leading_digit_is_two(idx_t k)
{
    if (k <= 7)          /* lookup for k = 0 … 7 */
        return LD_SMALL[k] == 2 ? 1 : 0;

    /* 1. compute fractional part of k·log10(φ) – log10(√5) in fixed point */
    unsigned __int128 prod   = (unsigned __int128)k * LOG10_PHI_SCALED;
    uint64_t    prod_mod   = (uint64_t)(prod % SCALE);
    uint64_t    r;
    if (prod_mod >= LOG10_SQRT5_SCALED)
        r = prod_mod - LOG10_SQRT5_SCALED;
    else
        r = prod_mod + SCALE - LOG10_SQRT5_SCALED;   /* add SCALE to avoid negative */

    /* 2. compare the fractional part with the boundaries for leading digit 2 */
    return (r >= TH2_LOWER && r < TH2_UPPER) ? 1 : 0;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 0; k <= n; ++k) {
        count += leading_digit_is_two(k);
    }
    return count;
}
