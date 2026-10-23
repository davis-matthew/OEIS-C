/*
 * A183867  a(n) = n + floor(2*sqrt(n)); complement of A184676.
 * https://oeis.org/A183867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(2*sqrt(n))
 * Range:     u8 n <= 225, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for all n >= 1 using only integer arithmetic. */
#include <stdint.h>
#include <stdbool.h>

/* Integer square root for idx_t (unsigned 64‑bit).  Returns ⌊√n⌋. */
static inline idx_t _isqrt(idx_t n)
{
    /* Edge cases */
    if (n == 0 || n == 1)
        return (idx_t)n;

    idx_t low = 0;
    idx_t high = n;
    idx_t ans = 0;

    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Compute floor(sqrt(n)) */
    idx_t k = _isqrt(n);

    /* Threshold k^2 + k + 1 (using 128‑bit for safety) */
    unsigned __int128 kk = (unsigned __int128)k * k;
    idx_t threshold = (idx_t)(kk + k + 1);

    /* Determine whether to add 1 to 2*k (i.e., whether sqrt(n) has fractional part ≥ 0.5) */
    idx_t add = (n >= threshold) ? 1 : 0;
    add += 2 * k; /* 2*k + add where add is 0 or 1 */

    /* Return n + floor(2*sqrt(n)) */
    return (seq_t)n + (seq_t)add;
}
