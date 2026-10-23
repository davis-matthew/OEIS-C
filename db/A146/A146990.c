/*
 * A146990  Triangle, read by rows, T(n, k) = binomial(n, k) for n < 2 and
 *          binomial(n, k) + n^(n-1) * binomial(n-2, k-1) otherwise.
 * https://oeis.org/A146990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, T(r,k) =
 *            binomial(r,k) for r < 2; T(r,k) = binomial(r,k) + r^(r-1) *
 *            binomial(r-2,k‑1) otherwise. Row index r =
 *            floor((sqrt(8*n+1)-1)/2), column k = n - r*(r+1)/2
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 56, u64 n <= 124,
 *            u128 n <= 331, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Find the greatest r such that r*(r+1)/2 <= n.  n is 64‑bit. */
static idx_t triangular_row(idx_t n)
{
    idx_t low = 0;
    idx_t high = 4294967295ULL;          /* 2^32‑1, enough for all n */
    idx_t best = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            best = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return best;
}

/* Compute the entry of the triangle for row r and column k. */
static inline seq_t value_of_row_col(idx_t r, idx_t k)
{
    if (r < 2) {
        return binomial(r, k);
    }
    seq_t bin = binomial(r, k);
    if (k == 0) {
        return bin;                     /* binomial(r-2,-1) is zero */
    }
    seq_t bin2 = binomial(r - 2, k - 1);
    /* r^(r-1) can be huge; let big_pow handle arbitrary precision */
    seq_t r_pow = big_pow((seq_t)r, (seq_t)(r - 1));
    return bin + r_pow * bin2;
}

seq_t a(idx_t n)
{
    idx_t r = triangular_row(n);
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;
    return value_of_row_col(r, k);
}
