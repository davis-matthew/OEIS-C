/*
 * A211603  Triangular array read by rows: T(n,k) is the number of
 *          n-permutations that are pure cycles having exactly k fixed points;
 *          n>=2, 0<=k<=n-2.
 * https://oeis.org/A211603
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 79, u64 n <= 211,
 *            u128 n <= 596, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A211603: a(n) is the n-th entry of the triangular array T(n,k)
 * read by rows (offset 2).  For the appropriate row r and column k,
 * T(r,k) = C(r,k)·(r-k-1)!.
 * This implementation uses only integer arithmetic and the binomial
 * helper from utils.  It works for any idx_t/seq_t combination
 * supported by the build (plain unsigned integer types or an
 * arbitrary‑precision class).
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square root: floor(sqrt(x)) for 0 ≤ x < 2^64 */
static inline idx_t floor_isqrt_u64(idx_t x)
{
    idx_t low = 0;
    idx_t high = (idx_t)1 << 32;      /* sqrt(2^64-1) < 2^32 */

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t m2 = (__uint128_t)mid * mid;
        if (m2 <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Computes the n-th term of A211603 (n ≥ 2).  For n < 2 the function
 * returns 0 (undefined behavior for the sequence). */
seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined section of the sequence */

    /* Position in the flattened triangle (1‑based) */
    idx_t pos = n - 1;

    /* Find the minimal r such that r*(r-1)/2 ≥ pos */
    idx_t r = (idx_t)((1 + floor_isqrt_u64(1 + 8 * pos)) >> 1);
    /* Adjust if the calculation was off by one (safe for all n) */
    if ( (__uint128_t)r * (r - 1) / 2 < pos ) ++r;

    /* Number of entries before row r */
    idx_t prev = (r - 1) * (r - 2) / 2;

    /* Column index (0‑based) in row r */
    idx_t k = pos - prev - 1;

    /* Compute (r-k-1)! iteratively */
    seq_t fact = 1;
    for (idx_t i = 1; i <= r - k - 1; ++i)
        fact *= (seq_t)i;

    /* Compute the binomial coefficient C(r,k) */
    seq_t comb = binomial(r, k);

    return comb * fact;
}
