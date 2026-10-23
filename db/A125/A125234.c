/*
 * A125234  Triangle T(n,k) read by rows: the k-th column contains the k-fold
 *          iterated partial sum of A000566.
 * https://oeis.org/A125234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes the flattened Heptagonal-number triangle (OEIS A125234).
 * Row r has n terms T(r,0)...T(r,r-1).  The k-th column contains the
 * k-fold iterated partial sum of the heptagonal numbers H(i)=i*(5*i-3)/2.
 *
 * For a given flattened index n (offset 1), find the row r such that
 * r*(r+1)/2 >= n.  Let k = n - r*(r-1)/2 - 1 (0‑based column).
 * If k==0 return H(r).  Otherwise
 *  T(r,k) = sum_{j=1}^{r-k} C(r-j-1, k-1) * H(j)   (*)
 * where C(a,b)=binomial(a,b) (provided in utils).
 *
 * Complexity: O(r) per call, where r ≈ sqrt(2n); for n≤10 000, r≤141.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n < 1U) return (seq_t)0;
    /* Find row r: smallest r with r*(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t s = mid * (mid + 1) / 2;
        if (s >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;
    idx_t s_prev = (r - 1) * r / 2;
    idx_t k = n - s_prev - 1;               /* 0‑based column */

    /* Heptagonal number H(r) */
    if (k == 0U) {
        seq_t j = (seq_t)r;
        return j * ((seq_t)5 * j - (seq_t)3) / (seq_t)2;
    }

    /* Use formula (*) */
    seq_t res = 0;
    idx_t K = k - 1U;
    for (idx_t j = 1U; j <= r - k; ++j) {
        idx_t N = r - j - 1U;
        seq_t C = binomial(N, K);
        seq_t jj = (seq_t)j;
        seq_t hj = jj * ((seq_t)5 * jj - (seq_t)3) / (seq_t)2; /* H(j) */
        res += C * hj;
    }
    return res;
}
