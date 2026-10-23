/*
 * A174037  Triangle T(n, k, q) = Sum_{j>=0} q^j * floor(binomial(n, k)/2^j)
 *          with q = 2, read by rows.
 * https://oeis.org/A174037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   a(n) = T(row_of(n), col(n), q=2) where T(n,k,q) = Σ_{j≥0} q^j *
 *            floor(binomial(n,k)/2^j). This function computes the triangular
 *            array entries of A174037 and flattens them row by row
 * Range:     u8 n <= 38, u16 n <= 126, u32 n <= 508, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper declarations */
#include <stdint.h>
#include <limits.h>
#include "../utils/binom.h"    /* provides seq_t binom(idx_t a, idx_t b); */

/* Return the largest r such that r*(r+1)/2 <= n. */
static inline idx_t row_of(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;

    /* Find an upper bound for the row. */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        low = high;
        high <<= 1;
        if (high == 0) {          /* overflow: clamp to maximum */
            high = UINT64_MAX;
            break;
        }
    }

    /* Binary search in [low, high] for the largest r with triangular(r) <= n. */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the (r,k) coordinates of the n‑th element. */
    idx_t r     = row_of(n);
    seq_t start = (seq_t)r * (r + 1) / 2;
    idx_t k     = n - (idx_t)start;

    /* Binomial coefficient C(r,k). */
    seq_t C = binom(r, k);

    /* Compute T(r,k,2) = Σ_{j≥0} 2^j * floor(C / 2^j). */
    seq_t res = 0;
    seq_t pow = 1;                 /* 2^j */
    seq_t div = C;                 /* floor(C / 2^j) */
    while (div > 0) {
        res += pow * div;
        pow <<= 1;
        div >>= 1;
    }
    return res;
}
