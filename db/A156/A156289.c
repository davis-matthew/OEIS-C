/*
 * A156289  Triangle read by rows: T(n,k) is the number of end rhyme patterns
 *          of a poem of an even number of lines (2n) with 1<=k<=n evenly
 *          rhymed sounds.
 * https://oeis.org/A156289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row index such that * (r-1)*r/2 < n
 *            ≤ r*(r+1)/2, k = n - (r-1)*r/2. T(n,k) is defined by * T(n,1)=1,
 *            T(n,k)=0 for n<k, and for 1<k≤n * T(n,k) = (2k-1)*T(n-1,k-1) +
 *            k^2*T(n-1,k)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 48, u64 n <= 111,
 *            u128 n <= 285, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find the row index r (1 ≤ r) such that r*(r+1)/2 ≥ n. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index within the row. */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;          /* 1 ≤ k ≤ r */

    /* Build the triangle up to row r using two temporary rows. */
    size_t sz = (size_t)(r + 1);     /* allocate indices 0..r */
    seq_t *prev = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *curr = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!prev || !curr)
        return 0; /* allocation failure, return 0 as fallback */

    /* Row 1 */
    prev[1] = 1;
    if (r == 1) {
        seq_t result = prev[(size_t)k];
        free(prev);
        free(curr);
        return result;
    }

    /* Build rows 2..r */
    for (idx_t row = 2; row <= r; ++row) {
        curr[1] = 1;
        for (idx_t col = 2; col <= row; ++col) {
            seq_t term1 = (seq_t)(2 * col - 1) * prev[(size_t)(col - 1)];
            seq_t term2 = (seq_t)(col * col) * prev[(size_t)col];
            curr[(size_t)col] = term1 + term2;
        }
        /* Swap prev and curr for next iteration. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[(size_t)k];
    free(prev);
    free(curr);
    return result;
}
