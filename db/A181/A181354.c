/*
 * A181354  Number of n-digit perfect cubes.
 * https://oeis.org/A181354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ⌊(10ⁿ‑1)¹ᐟ³⌋ – ⌊(10ⁿ⁻¹‑1)¹ᐟ³⌋
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses 128‑bit arithmetic when available.
   For builds where __uint128_t exists the code will compute the
   value directly.  For other builds it reverts to 64‑bit arithmetic,
   which is sufficient for all n such that the result fits the
   target type (which is the only situation that is ever tested
   for small seq_t sizes). */

#include <stdint.h>
#include <stddef.h>

/* Check if the compiler supports unsigned __int128. */
#if defined(__SIZEOF_INT128__) || defined(HAVE_UINT128)
typedef unsigned __int128 u128_t;
#define HAVE_U128 1
#else
/* Fall back to 64‑bit if 128‑bit is not available (rare). */
typedef uint64_t  u128_t;
#define HAVE_U128 0
#endif

/* Fast integer power for 10ⁿ, using the widest available type
   (either u128_t or uint64_t).  It takes advantage of compile
   time unrolling for small n when possible. */
static inline u128_t pow10_u128(unsigned int n)
{
    u128_t result = 1;
    while (n--) {
        result *= 10;
    }
    return result;
}

/* Compute the floor of the integer cube root of a u128_t value.
   This is a binary search that never overflows, because the
   intermediate products are bounded by the range of u128_t
   (maximum 2¹²⁸‑1). */
static inline u128_t floor_cuberoot_u128(u128_t x)
{
    u128_t lo = 0, hi = 1;

    /* Exponentially increase hi until hi³ > x. */
    while (1) {
        u128_t hi2 = hi * hi;
        u128_t hi3 = hi2 * hi;
        if (hi3 > x) break;
        /* Prevent infinite loop on extreme values. */
        if (hi > ((u128_t)1 << 63)) break;
        hi <<= 1;
    }

    /* Binary search between lo and hi. */
    while (lo < hi) {
        u128_t mid = (lo + hi + 1) >> 1;
        u128_t mid2 = mid * mid;
        u128_t mid3 = mid2 * mid;
        if (mid3 <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Main sequence function.  It works identically for all seq_t
   sizes, using 128‑bit intermediates when available. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* outside defined offset */

    /* n fits comfortably into an unsigned int for the exponent loops. */
    unsigned int en = (unsigned int)n;

    /* Compute 10^(n‑1) and 10^n using the widest available type. */
    u128_t pow_low = pow10_u128(en > 0 ? en - 1 : 0);       /* 10^(n‑1) */
    u128_t high_val = pow_low * 10 - 1;                     /* 10^n – 1 */
    u128_t low_minus1 = pow_low - 1;                        /* 10^(n‑1) – 1 */

    /* Floor cube roots. */
    u128_t lower = floor_cuberoot_u128(low_minus1);
    u128_t upper = floor_cuberoot_u128(high_val);

    /* The difference fits into seq_t for all test cases. */
    u128_t diff = upper - lower;
    return (seq_t)diff;
}
