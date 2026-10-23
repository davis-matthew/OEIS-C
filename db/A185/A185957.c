/*
 * A185957  Second accumulation array of the array min{n,k}, by antidiagonals.
 * https://oeis.org/A185957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry of the symmetric matrix M = T' * T (the
 *            “second accumulation array” of min{n,k}) listed by
 *            antidiagonals. For i+j = d where d is the smallest integer with
 *            (d‑1)d/2 ≥ n, the offset on that diagonal is offset = n –
 *            (d‑2)(d‑1)/2 giving i = offset, j = d – offset. Each matrix
 *            entry...
 * Range:     u8 n <= 39, u16 n <= 478, u32 n <= 1275, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#if defined(_MSC_VER) && !defined(__cplusplus)
  #define UINT64_MAX 0xFFFFFFFFFFFFFFFFULL
#endif

static inline seq_t triangular(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn + 1) / 2;
}

static inline idx_t diagonal_from_index(idx_t n)
{
    /* find smallest d such that (d-1)d/2 >= n  (n >= 1) */
    idx_t low = 2;
    idx_t high;
    if (n >= UINT64_MAX - 1)
        high = UINT64_MAX;
    else
        high = n + 1;      /* high is always >= low for n >= 1 */

    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        /* use 128‑bit to avoid overflow */
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low; /* d */
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined, return 0 as a placeholder */
        return 0;

    /* locate diagonal and coordinates (i,j) */
    idx_t d = diagonal_from_index(n);
    unsigned __int128 prev_terms128 = (unsigned __int128)(d - 2) * (d - 1) / 2;
    idx_t prev_terms = (idx_t)prev_terms128;
    idx_t offset = n - prev_terms; /* 1 … d-1 */
    idx_t i = offset;
    idx_t j = d - offset;

    /* compute M[i][j] */
    idx_t min_ij = (i < j) ? i : j;
    seq_t sum = 0;
    for (idx_t k = 1; k <= min_ij; ++k) {
        idx_t m = i - k + 1;
        idx_t r = j - k + 1;
        seq_t tri_m = triangular(m);
        seq_t tri_r = triangular(r);
        sum += tri_m * tri_r;
    }
    return sum;
}
