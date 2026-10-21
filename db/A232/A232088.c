/*
 * A232088  Table read by rows: Replace last term of the n-th row with
 *          1,1,2,4,...,2^n to get the next row.
 * https://oeis.org/A232088
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) is obtained from the recursive table where each row n is
 *            formed by * removing the last term of row n‑1 and appending the
 *            list 1,1,2,4,…,2^n. * The sequence is the concatenation of all
 *            rows
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t r    = 0;   /* current row index   */
    idx_t cum  = 0;   /* total length of all rows before row r */

    /* Find the row that contains index n */
    while (1)
    {
        /* length of row r : 1 + r*(r+1)/2 */
        idx_t L = 1 + (idx_t)(((unsigned __int128)r * (r + 1)) / 2);
        if (cum + L > n)        /* this row reaches or passes n */
            break;
        cum += L;
        r++;
    }

    idx_t pos = n - cum;   /* position inside row r */

    /* Find the value at this position by walking backwards
     * until we hit the part that comes from seq_r. */
    while (r > 0)
    {
        idx_t prev_len = 1 + (idx_t)(((unsigned __int128)(r - 1) * r) / 2);
        if (pos < prev_len - 1)   /* the value is in the truncated part of row r-1 */
        {
            r--;
            continue;
        }

        /* The value is in the appended sequence of row r */
        idx_t idx_seq = pos - (prev_len - 1);   /* index inside seq_r */
        if (idx_seq == 0 || idx_seq == 1) {
            return 1;
        }
        /* idx_seq >= 2 => 2^(idx_seq-1) */
        return pow2(idx_seq - 1);
    }

    /* r == 0 only occurs when n == 0 */
    return 1;
}
