/*
 * A190909  Triangle read by rows: T(n,k) = binomial(n+k,n-k) * k! /
 *          floor(k/2)!^2.
 * https://oeis.org/A190909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Flattened triangle T(n,k) with
 *        T(r,k) = binomial(r+k,r-k) * k! / floor(k/2)!^2
 * where r = floor((sqrt(8n+1)-1)/2) and k = n - r(r+1)/2. */
 
#include <stdint.h>

/*----- Integer square root for uint64_t -----------------------------------*/
static inline idx_t _isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/*----- binomial coefficient (n choose k) ---------------------------------*/
static seq_t _binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/*----- factorial ----------------------------------------------------------*/
static seq_t _fact(idx_t m)
{
    seq_t res = 1;
    for (idx_t i = 2; i <= m; ++i) {
        res *= (seq_t)i;
    }
    return res;
}

/*----- main sequence function --------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find the row r containing index n */
    idx_t d = _isqrt_u64(8 * (idx_t)n + 1);
    idx_t r = (d - 1) >> 1;          /* floor((sqrt(8n+1)-1)/2) */

    /* Position k within that row */
    idx_t k = n - r * (r + 1) / 2;

    /* Binomial part: binomial(r+k, r-k) */
    seq_t bin = _binom(r + k, r - k);

    /* Factorial ratio: k! / ( floor(k/2)! )^2 */
    seq_t fact_k = _fact(k);
    seq_t fact_half = _fact(k >> 1);   /* floor(k/2) = k/2 for integer division */
    seq_t denom = fact_half * fact_half;
    seq_t ratio = fact_k / denom;

    return bin * ratio;
}
