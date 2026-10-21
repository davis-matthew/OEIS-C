/*
 * A221215  T(n,k)= ((n+k)^2-2*(n+k)+4-(n+3*k-2)*(-1)^(n+k))/2; n , k > 0,
 *          read by antidiagonals.
 * https://oeis.org/A221215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) computed by traversing the table T(i,j) with T(i,j) =
 *            ((i+j)^2 - 2*(i+j) + 4 - (i+3*j-2)*(-1)^(i+j)) / 2 The sequence
 *            a(n) is obtained by reading the table by antidiagonals: first
 *            T(1,1), then T(1,2), T(2,1), T(1,3), T(2,2), T(3,1),
 * Range:     u8 n <= 149, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t diag_from_index(idx_t n) {
    /* Find the smallest m such that m*(m+1)/2 >= n.
       m is the diagonal index (s = m+1 == i+j). */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low; /* m */
}

seq_t a(idx_t n) {
    /* Map index n (1‑based) to (i,j) and compute T(i,j). */
    idx_t m = diag_from_index(n);   /* diagonal index, s = m+1 */
    idx_t t = m - 1;                 /* t = s-2 */
    unsigned __int128 prevCount128 = (unsigned __int128)t * (t + 1) / 2;
    idx_t prevCount = (idx_t)prevCount128;
    idx_t offset = n - prevCount - 1;        /* 0‑based index inside the diagonal */

    idx_t i = offset + 1;                       /* row */
    idx_t j = t + 1 - offset;                   /* column */

    /* Compute T(i,j) using only seq_t operations. */
    seq_t ni = (seq_t)i;
    seq_t kj = (seq_t)j;
    seq_t sum = ni + kj;
    seq_t val1 = sum * sum - 2 * sum + 4;        /* (i+j)^2 - 2*(i+j) + 4 */
    seq_t part = ni + 3 * kj - 2;                /* i + 3*j - 2 */

    seq_t result;
    if (sum & 1) {                              /* odd sum: + part */
        result = (val1 + part) / 2;
    } else {                                   /* even sum: - part */
        result = (val1 - part) / 2;
    }
    return result;
}
