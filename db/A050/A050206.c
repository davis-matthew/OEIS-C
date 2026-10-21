/*
 * A050206  Triangle read by rows: smallest denominator of the expansion of
 *          k/n using the greedy algorithm, 1<=k<=n-1.
 * https://oeis.org/A050206
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl, changed
 * Formula:   not stated
 * Range:     u8 n <= 9871, u16 n <= 9871, u32 n <= 9871, u64 n <= 9871,
 *            u128 n <= 9871, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: a(n) is the sequence obtained by
   flattening T(r,k)=ceil(r/k) for rows r≥2, 1≤k≤r−1
   (offset 2, so a(2) is the first element). */
#include <stdint.h>
#include <stdbool.h>

static inline idx_t triangular(idx_t r) {
    /* returns (r-1)*r/2, with r≥2 */
    return (r - 1) * r / 2;
}

seq_t a(idx_t n) {
    /* n≥2 by specification */
    idx_t l = n - 1;                 /* linear index starting at 1 for n=2 */
    /* find the row r such that triangular(r-1) < l ≤ triangular(r) */
    idx_t low = 2, high = 2;
    while (triangular(high) < l)      /* increase high until it covers l */
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= l)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                    /* row number */
    idx_t prev = triangular(r - 1);   /* number of elements before this row */
    idx_t k = l - prev;               /* position within the row (1‑based) */
    /* smallest denominator of the greedy expansion of k/r is ceil(r/k) */
    return (seq_t)((r + k - 1) / k);
}
