/*
 * A135818  Number of 1's (or A's) in the Wythoff representation of n.
 * https://oeis.org/A135818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 318, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is computed via the unique Wythoff representation of n.
 * For n = 1, a(1) = 1.
 * For n > 1, let k be such that n = A(k) or n = B(k).
 *   If n = A(k) then a(n) = a(k) + 1.
 *   If n = B(k) and k = 1 (i.e., n = 2) then a(n) = 0.
 *   If n = B(k) with k > 1 then a(n) = a(k).
 * The sequences A(k) = ⌊k·φ⌋ and B(k) = A(k) + k are the lower and upper
 * Beatty sequences of √5.  The floor can be obtained from integers only.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) for 128‑bit x using binary search. */
static inline idx_t isqrt_u128(unsigned __int128 x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)-1;              /* maximum 64‑bit value */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

/* Compute A(k) = floor(k * φ) without floating point. */
static inline seq_t A_at(idx_t k)
{
    if (k == 0) return 0;
    unsigned __int128 prod = (unsigned __int128)5 * (unsigned __int128)k * (unsigned __int128)k;
    idx_t sqrt5k = isqrt_u128(prod);            /* floor(k * sqrt(5)) */
    return (seq_t)((k + sqrt5k) / 2);           /* floor((k + k*sqrt(5))/2) */
}

/* Compute B(k) = A(k) + k. */
static inline seq_t B_at(idx_t k)
{
    return A_at(k) + k;
}

/* Find the parent k of n in the Wythoff sequences. Returns 0 if not found. */
static inline idx_t parent_in_A(idx_t n)
{
    idx_t low = 1, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t val = A_at(mid);
        if (val == n) return mid;
        if (val < n)  low = mid + 1;
        else          high = mid - 1;
    }
    return 0;
}

static inline idx_t parent_in_B(idx_t n)
{
    idx_t low = 1, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t val = B_at(mid);
        if (val == n) return mid;
        if (val < n)  low = mid + 1;
        else          high = mid - 1;
    }
    return 0;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Base case according to OEIS data. */
    if (n == 1) return (seq_t)1;

    /* Check if n is in the A sequence. */
    idx_t k = parent_in_A(n);
    if (k != 0) {
        return a(k) + (seq_t)1;
    }

    /* Otherwise, n must be in the B sequence. */
    k = parent_in_B(n);
    if (k != 0) {
        /* Special case: n = B(1) = 2 should give 0. */
        if (k == 1) return (seq_t)0;
        return a(k);
    }

    /* If we reach here, n is not representable (should not happen). */
    return (seq_t)0;
}
