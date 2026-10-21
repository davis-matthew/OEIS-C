/*
 * A083050  Square table read by antidiagonals forms a permutation of the
 *          natural numbers: T(n,0) = floor(n*x/(x-1))+1, T(n,k+1) =
 *          ceiling(x*T(n,k)), where x = sqrt(2), n>=0, k>=0.
 * https://oeis.org/A083050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the “antidiagonal” read from the infinite table * T(n,0)
 *            = 1 + 2*n + floor(n*sqrt(2)) * T(n,k+1) = ceil(sqrt(2)*T(n,k)) =
 *            floor(sqrt(2)*T(n,k)) + 1 * (x = sqrt(2)). The sequence
 *            a(0),a(1),… is the permutation obtained * by reading the table
 *            row‑by‑row, column‑by‑column in antidiagonals, *...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

/* ------------------------------------------------------------------ */
/* Integer square–root for 128–bit unsigned values.  The result is
 * cast to unsigned long long.  The routine is used only when the
 * result is known to fit into 64 bits (i.e. for small seq_t types). */
/* ------------------------------------------------------------------ */
static inline unsigned long long floor_sqrt_uint128(unsigned __int128 v)
{
    unsigned __int128 low = 0;
    unsigned __int128 high = (unsigned __int128)~(unsigned long long)0; /* UINT64_MAX */
    unsigned long long ans = 0;
    while (low <= high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 sq  = mid * mid;
        if (sq <= v) {
            ans = (unsigned long long)mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* ------------------------------------------------------------------ */
/* Computes floor(t * sqrt(2)) for an arbitrary seq_t value.  The
 * implementation chooses a fast 128‑bit routine when seq_t is small
 * (≤8 bytes) and a general seq_t routine when seq_t is larger.  The
 * returned result is a seq_t value. */
/* ------------------------------------------------------------------ */
static inline seq_t floor_mul_sqrt(seq_t t)
{
    /* For seq_t small enough, use 128‑bit arithmetic on a narrow type. */
    if (sizeof(seq_t) <= 8) {
        unsigned long long tt = (unsigned long long)t;      /* t fits into 64 bits */
        unsigned __int128 prod = (unsigned __int128)2 * tt * tt;
        unsigned long long res = floor_sqrt_uint128(prod);
        return (seq_t)res;
    }
    /* For larger seq_t use the provided floor_sqrt on a product that
     * certainly does not overflow the seq_t type. */
    seq_t v = (seq_t)2 * t * t;
    return floor_sqrt(v);
}

/* ------------------------------------------------------------------ */
/* Find the antidiagonal indexed by a(n).  Let d be the largest integer
 * such that d*(d+1)/2 ≤ n.  The index inside that diagonal is
 * i = n - d*(d+1)/2 (rows, i increases along the diagonal).  Then
 * j = d - i is the column index.  The value T(i,j) is then computed
 * using the table recurrence. */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Find diagonal d : d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        if ((__int128)mid * (mid + 1) / 2 <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    idx_t d = low;

    /* Position inside the diagonal: row i, column j */
    idx_t i = n - d * (d + 1) / 2;
    idx_t j = d - i;

    /* Compute T(i,0) = 1 + 2*i + floor(i*sqrt(2)) */
    seq_t t = (seq_t)2 * (seq_t)i + floor_mul_sqrt((seq_t)i) + 1;

    /* Iterate j times: T(i,k+1) = floor(sqrt(2)*T(i,k)) + 1 */
    for (idx_t k = 0; k < j; ++k) {
        t = floor_mul_sqrt(t) + 1;
    }
    return t;
}
