/*
 * A181783  Array described in comments to A053482, here read by increasing
 *          antidiagonals. See comments below.
 * https://oeis.org/A181783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 47, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A181783
    a(i,j) = i*(j-1)*a(i-1,j) + a(i,j-1)  for i,j > 0
    a(i,0) = a(0,j) = 1
    The sequence a(n) is obtained by reading the array by increasing
    antidiagonals (s = i+j) and, for each diagonal, listing the cells
    from i = s down to 0 (i.e. from higher row to lower row).  */
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

static seq_t *dp = NULL;  /* 2‑D table stored row‑wise, dp[i*width + j] */
static idx_t width = 0;   /* current table width = max_s + 1 */

/* Build table up to at least diagonal 'max_s' (i.e. width = max_s+1) */
static void ensure_table(idx_t max_s)
{
    if (dp && max_s < width - 1)
        return;                          /* already enough */

    idx_t new_size = max_s + 1;           /* new width */

    seq_t *newdp = alloc_seq_array(new_size * new_size);
    if (!newdp)                           /* allocation failed – caller will abort */
        return;

    /* compute all entries of the new table */
    for (idx_t i = 0; i < new_size; ++i) {
        for (idx_t j = 0; j < new_size; ++j) {
            if (i == 0 || j == 0) {
                newdp[i * new_size + j] = 1;
            } else {
                seq_t mult = (seq_t)i * (seq_t)(j - 1);
                mult = mult * newdp[(i - 1) * new_size + j];
                newdp[i * new_size + j] = mult + newdp[i * new_size + (j - 1)];
            }
        }
    }

    /* replace old table */
    if (dp) free(dp);
    dp = newdp;
    width = new_size;
}

/* Return the n‑th term of A181783 (offset 0) */
seq_t a(idx_t n)
{
    /* locate diagonal s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t low = 0, high = n, s = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        idx_t d = mid * (mid + 1) / 2;
        if (d <= n) {
            low = mid + 1;
            s = mid;
        } else {
            high = mid - 1;
        }
    }

    idx_t pos = n - s * (s + 1) / 2;  /* position within the diagonal */
    idx_t i = s - pos;                /* row index   */
    idx_t j = pos;                    /* column index*/

    ensure_table(s);                  /* compute up to this diagonal */
    return dp[i * width + j];
}
