/*
 * A068777  S(n; 0,3) = S(n; 2,1) where S(n; t,s) is the number of length n
 *          4-ary strings whose digits sum to t mod 4 and whose sum of
 *          products of all pairs of digits sum to s mod 4.
 * https://oeis.org/A068777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 4‑ary strings of length n whose * digit sum ≡ 0
 *            (mod 4) and whose sum of pairwise products ≡ 3 (mod 4). *
 *            Computed via dynamic programming over the pair (t,s) where * t =
 *            (sum of digits) mod 4 and s = (sum of pairwise products) mod 4.
 *            * Recurrence: for each previous state (t',s')...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Arrays hold counts for each of the 4×4 = 16 (t,s) residue classes.
     * prev holds values for strings of current length-1,
     * curr holds values for strings of current length.  */
    seq_t prev[16] = {0};
    seq_t curr[16];

    /* Base case: empty string of length 0 has t=0, s=0. */
    prev[0] = 1;

    for (idx_t len = 1; len <= n; ++len) {
        /* clear current array */
        for (int i = 0; i < 16; ++i) curr[i] = 0;

        for (int t = 0; t < 4; ++t) {
            for (int s = 0; s < 4; ++s) {
                seq_t cnt = prev[t * 4 + s];
                if (!cnt) continue;

                /* Append each digit d = 0..3 */
                for (int d = 0; d < 4; ++d) {
                    int nt = (t + d) & 3;           /* (t' + d) % 4  */
                    int ns = (s + d * t) & 3;       /* (s' + d*t') % 4 */
                    curr[nt * 4 + ns] += cnt;
                }
            }
        }

        /* move to next length */
        for (int i = 0; i < 16; ++i) prev[i] = curr[i];
    }

    /* a(n) is the count for (t=0, s=3) */
    return prev[0 * 4 + 3];
}
