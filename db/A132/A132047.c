/*
 * A132047  3*A007318 - 2*A103451 as infinite lower triangular matrices.
 * https://oeis.org/A132047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the flattened lower‑triangular matrix * whose entry in
 *            row r and column k (0≤k≤r) equals * 1 if k = 0 or k = r,
 *            otherwise 3*binomial(r,k). * The linear index n satisfies *
 *            T(r,0)=0 < n < T(r+1,0)= (r+1)(r+2)/2, where * T(r,0)=r(r+1)/2
 *            is the starting index of row r
 * Range:     u8 n <= 39, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for (int) cast, if needed */

/* Compute binomial coefficient nCk as a seq_t, using exact division. */
static inline seq_t binom_calc(idx_t n, idx_t k)
{
    if (k > n - k)                /* use symmetry nCk = nC(n-k) */
        k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        /* multiply by (n - k + i) then divide by i */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;    /* division is exact */
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t low  = (idx_t)0;
    idx_t high = (idx_t)1;
    while (((__int128)high * (high + (idx_t)1)) / (idx_t)2 <= n) {
        high <<= (idx_t)1;      /* double until we surpass n */
    }
    while (low + (idx_t)1 < high) {
        idx_t mid = low + ((high - low) >> (idx_t)1);
        if (((__int128)mid * (mid + (idx_t)1)) / (idx_t)2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;

    /* Column k is the offset within the row. */
    idx_t offset = (idx_t)((( __int128)r * (r + (idx_t)1)) / (idx_t)2);
    idx_t k = n - offset;

    /* The edge elements are 1. */
    if (k == (idx_t)0 || k == r)
        return (seq_t)1;

    /* Interior elements are 3 * binomial(r, k). */
    seq_t val = binom_calc(r, k);
    return val * (seq_t)3;
}
