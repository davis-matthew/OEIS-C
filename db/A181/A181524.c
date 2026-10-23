/*
 * A181524  Table read by rows in which n-th row gives the smallest solution
 *          (a,b,c,d) to the partition (4n+1)^2 = a^2 + b^2 + c^2 + d^2 with 0
 *          < a < b < c < d.
 * https://oeis.org/A181524
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 121, u32 n <= 157, u64 n <= 157, u128 n <= 157,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A181524 – Table read by rows.
    For each n ≥ 2 the function returns the n‑th element of the table
    consisting of the 4‑tuples (a,b,c,d) that solve
        (4r+1)^2 = a^2 + b^2 + c^2 + d^2     (0 < a < b < c < d)
    with r = 2,3,4,…, and the tuple is chosen
    lexicographically minimal: smallest a, then smallest b, then c, then d.
    The table is read row‑by‑row and flattened: the first row (r=2)
    yields terms 2,4,5,6, the second row (r=3) yields 1,2,8,10, etc.
*/

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* compute the minimal solution (a,b,c,d) for a given row r (r≥2) */
static void minimal_partition(idx_t r, seq_t *outA, seq_t *outB, seq_t *outC, seq_t *outD)
{
    seq_t four_r = (seq_t)4 * (seq_t)r;
    seq_t target = (four_r + 1) * (four_r + 1);

    for (idx_t a_i = 1; ; ++a_i) {
        seq_t a2 = (seq_t)a_i * a_i;
        if (a2 >= target) break; /* should never happen for valid rows */

        for (idx_t b_i = a_i + 1; ; ++b_i) {
            seq_t b2 = (seq_t)b_i * b_i;
            seq_t min_c = (seq_t)b_i + 1;
            seq_t min_d = (seq_t)b_i + 2;
            seq_t min_cd = min_c * min_c + min_d * min_d;

            if (a2 + b2 + min_cd > target) break;  /* no b ≥ b_i works */

            seq_t rem = target - a2 - b2;          /* squares remaining for c and d */

            for (idx_t c_i = b_i + 1; ; ++c_i) {
                seq_t c2 = (seq_t)c_i * c_i;
                seq_t min_d2 = (c_i + 1) * (c_i + 1);

                if (c2 + min_d2 > rem) break;     /* no c ≥ c_i works */

                seq_t d2 = rem - c2;
                if (d2 <= c2) continue;          /* would give d ≤ c */

                seq_t d = floor_sqrt(d2);
                if (d*d == d2 && d > c_i) {
                    *outA = (seq_t)a_i;
                    *outB = (seq_t)b_i;
                    *outC = (seq_t)c_i;
                    *outD = d;
                    return;
                }
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;      /* sequence is undefined for n < 2 */

    /* map the global index n to the row r and the position within the row */
    idx_t g   = n - 2;           /* shift to 0‑based index */
    idx_t r   = 2 + g / 4;       /* row number (starts at 2)            */
    idx_t pos = g % 4;           /* 0→a, 1→b, 2→c, 3→d                 */

    seq_t a_val, b_val, c_val, d_val;
    minimal_partition(r, &a_val, &b_val, &c_val, &d_val);

    switch (pos) {
        case 0: return a_val;
        case 1: return b_val;
        case 2: return c_val;
        case 3: return d_val;
        default: return 0;  /* unreachable */
    }
}
