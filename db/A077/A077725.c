/*
 * A077725  a(n) = smallest square > 1 which can be expressed as a sum of
 *          distinct powers of n.
 * https://oeis.org/A077725
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A077725: smallest square > 1 that can be expressed as a sum of distinct powers of n.
 * For all n ≥ 2 the rule is
 *   a(n) = n^2                  if n is neither a perfect square nor one less than a perfect square,
 *   a(n) = k^2                  if n = k^2,
 *   a(n) = k^2                  if n = k^2 – 1.
 * The function below implements this formula directly.
 */

#include <stdbool.h>

/* Integer floor square root for 64‑bit unsigned integers.
   The result is guaranteed not to overflow when used in this file. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;

    const idx_t max_root = ((idx_t)1 << 32) - 1;   /* sqrt(UINT64_MAX) */
    idx_t low  = 0;
    idx_t high = (n > max_root ? max_root : n);

    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if (mid > 0 && mid > n / mid)   /* mid*mid > n  (avoids overflow) */
            high = mid - 1;
        else
            low  = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* offset starts at 2; value below is irrelevant */

    seq_t ns = (seq_t)n;
    idx_t r  = floor_sqrt_u64(n);     /* floor sqrt of n */

    if (r * (idx_t)r == n)          /* n is a perfect square */
        return ns;                  /* a(n) = n */

    if ((r + 1U) * (r + 1U) == n + 1U) /* n = k^2 – 1 */
        return (seq_t)((r + 1U) * (r + 1U));

    return ns * ns;                  /* default case: a(n) = n^2 */
}
