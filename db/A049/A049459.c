/*
 * A049459  Generalized Stirling number triangle of first kind read by rows:
 *          T(n, k) = [x^k] Product_{m=1..n} (x - m - r), with r = 3.
 * https://oeis.org/A049459
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 9, i16 n <= 20, i32 n <= 65, i64 n <= 171, i128 n <= 527,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(0,0)=1; T(n,k)=T(n-1,k-1)-(n+3)*T(n-1,k);
   a(n) is the (n+1)-th entry when the triangle is read by rows
   (row 0 then row 1 etc.).  Thus a(n)=T(r,offset) where
   r = ⌊(√(8n+1)-1)/2⌋ and offset = n - r(r+1)/2. */

#include <stdlib.h>

/* integer square root for 64‑bit unsigned integers */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 mid2 = (__int128)mid * mid;
        if (mid2 == n)
            return mid;
        else if (mid2 < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* compute row r and offset within that row */
    idx_t t = (idx_t)(((__int128)8 * n) + 1);    /* 8*n+1, safe for n≤10^9 */
    idx_t sq = isqrt_u64(t);
    idx_t r = (sq - 1) / 2;

    idx_t sum_before = (idx_t)(((__int128)r * (r + 1)) / 2);
    idx_t offset = n - sum_before;              /* 0 ≤ offset ≤ r */

    /* base row 0 */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t));
    prev[0] = 1;

    /* build rows up to r */
    for (idx_t i = 1; i <= r; ++i) {
        idx_t cur_len = i + 1;
        seq_t *cur = (seq_t *)calloc(cur_len, sizeof(seq_t));
        seq_t factor = (seq_t)(i + 3);

        /* first element */
        cur[0] = -(factor * prev[0]);

        /* middle elements */
        for (idx_t k = 1; k < i; ++k)
            cur[k] = prev[k - 1] - factor * prev[k];

        /* last element is always 1 */
        cur[i] = prev[i - 1];

        free(prev);
        prev = cur;
    }

    seq_t result = prev[offset];
    free(prev);
    return result;
}
