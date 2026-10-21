/*
 * A216916  Triangle read by rows, T(n,k) for 0<=k<=n, generalizing A098742.
 * https://oeis.org/A216916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216916 produced by the triangle recurrence
   T(0,0) = 1;  T(0,k>0) = 0;
   T(n,k) = T(n-1,k-1) + (k+1)*T(n-1,k) + (k+2)*T(n-1,k+1)
   The flattened sequence a(n) is obtained by reading the
   triangle row by row: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), ...   */

#include <stdlib.h>
#include <stdint.h>

static seq_t *triangle = NULL;      /* flat storage of the triangle */
static idx_t  computed_r = 0;       /* highest row computed so far     */
static size_t computed_size = 0;    /* number of elements actually stored */

/* Binary search for the largest r with r*(r+1)/2 <= n   */
static idx_t
row_for_index(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            ans = mid;
            low = mid + 1;
        } else
            high = mid - 1;
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* ensure row 0 exists */
    if (!triangle) {
        triangle = (seq_t *)malloc(sizeof(seq_t));
        if (!triangle) { return 0; }      /* allocation failed */
        triangle[0] = (seq_t)1;
        computed_size = 1;
        computed_r   = 0;
    }

    /* find row r and column k for n   */
    idx_t r = row_for_index(n);
    idx_t offset_before = r * (r + 1) / 2;
    idx_t k = n - offset_before;

    /* extend the triangle up to row r if needed   */
    if (r > computed_r) {
        for (idx_t new_r = computed_r + 1; new_r <= r; ++new_r) {
            size_t new_size = (size_t)(new_r + 1) * (new_r + 2) / 2;
            if (new_size > computed_size) {
                seq_t *tmp = (seq_t *)realloc(triangle,
                                              new_size * sizeof(seq_t));
                if (!tmp) return 0;   /* reallocation failed */
                triangle     = tmp;
                computed_size = new_size;
            }

            idx_t prev_offset = (new_r - 1) * new_r / 2;
            idx_t curr_offset = new_r * (new_r + 1) / 2;

            for (idx_t col = 0; col <= new_r; ++col) {
                seq_t a1 = 0, a2 = 0, a3 = 0;
                if (col > 0)
                    a1 = triangle[prev_offset + (col - 1)];
                if (col <= new_r - 1)
                    a2 = triangle[prev_offset + col];
                if (col + 1 <= new_r - 1)
                    a3 = triangle[prev_offset + (col + 1)];
                triangle[curr_offset + col] =
                    a1 + (seq_t)(col + 1) * a2 + (seq_t)(col + 2) * a3;
            }
        }
        computed_r = r;
    }

    return triangle[offset_before + k];
}
