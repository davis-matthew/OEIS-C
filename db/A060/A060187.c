/*
 * A060187  Triangle read by rows: Eulerian numbers of type B, T(n,k) (1 <= k
 *          <= n) given by T(n, 1) = T(n,n) = 1, otherwise T(n, k) = (2*n -
 *          2*k + 1)*T(n-1, k-1) + (2*k - 1)*T(n-1, k).
 * https://oeis.org/A060187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice, changed
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 70, u64 n <= 177,
 *            u128 n <= 447, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,1)=T(n,n)=1; for 1<k<n: T(n,k)= (2n-2k+1)*T(n-1,k-1)+(2k-1)*T(n-1,k).  
   a(n) is the nth element of the triangle read by rows. */
#include <stdlib.h>

static seq_t **triangle = NULL;
static idx_t max_row = 0;

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find the row r with r*(r+1)/2 >= n */
    idx_t r = 1;
    while ((unsigned __int128)r * (r + 1) / 2 < n)
        ++r;

    idx_t before = (idx_t)((unsigned __int128)r * (r - 1) / 2);
    idx_t k = n - before;

    /* Compute rows up to r if not already done */
    if (r > max_row) {
        seq_t **new_tri = (seq_t**)realloc(triangle, sizeof(seq_t*) * (r + 1));
        if (!new_tri) abort();
        triangle = new_tri;

        for (idx_t i = max_row + 1; i <= r; ++i) {
            triangle[i] = (seq_t*)malloc(sizeof(seq_t) * (i + 1));
            if (!triangle[i]) abort();

            triangle[i][1] = 1;
            triangle[i][i] = 1;

            for (idx_t j = 2; j < i; ++j) {
                seq_t a_prev = triangle[i - 1][j - 1];
                seq_t b_prev = triangle[i - 1][j];
                seq_t val = ((seq_t)(2 * i - 2 * j + 1)) * a_prev
                          + ((seq_t)(2 * j - 1)) * b_prev;
                triangle[i][j] = val;
            }
        }
        max_row = r;
    }

    return triangle[r][k];
}
