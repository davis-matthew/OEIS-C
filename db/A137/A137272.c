/*
 * A137272  Number of 1D random walks with 8 steps where the median of the
 *          positions is n.
 * https://oeis.org/A137272
 *
 * Offset:    -4    (a(-4) is the first term)
 * Keywords:  easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of 8‑step simple random‑walk trajectories
   whose median position equals n.  The counts are obtained by brute‑force
   enumeration of all 2^8 = 256 possible walks.  (Median values range
   from -4 to 4 inclusive.) */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of counts for medians -4 .. 4 */
    static seq_t table[9];
    static bool init = false;

    if (!init) {
        /* initialise to zero */
        for (int i = 0; i < 9; ++i)
            table[i] = 0;

        /* enumerate all 256 walks */
        for (idx_t mask = 0; mask < 256; ++mask) {
            idx_t pos = 0;
            idx_t arr[9];
            arr[0] = 0;                       /* initial position */

            /* generate partial sums */
            for (int step = 0; step < 8; ++step) {
                if ((mask >> step) & 1)
                    pos += 1;                 /* step right */
                else
                    pos -= 1;                 /* step left */
                arr[step + 1] = pos;
            }

            /* sort the 9 positions (insertion sort, small fixed size) */
            for (int i = 1; i < 9; ++i) {
                idx_t key = arr[i];
                int j = i - 1;
                while (j >= 0 && arr[j] > key) {
                    arr[j + 1] = arr[j];
                    --j;
                }
                arr[j + 1] = key;
            }

            /* median = 5th smallest value (index 4 after sorting) */
            idx_t median = arr[4];
            idx_t idxn   = median + 4;         /* shift range -4..4 to 0..8 */

            if (idxn >= 0 && idxn < 9)
                ++table[(size_t)idxn];
        }

        init = true;
    }

    /* answer lookup */
    idx_t idx = n + 4;
    if (idx < 0 || idx >= 9)
        return (seq_t)0;
    return table[(size_t)idx];
}
