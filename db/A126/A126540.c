/*
 * A126540  Number of base 19 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of length‑n strings over the alphabet {0,…,18}
 *            where adjacent symbols differ by at most five. a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
/* use the library routine that constructs elements of seq_t */
#include "../utils/seq_alloc_array.h"


seq_t a(idx_t n)
{
    /* If no digits are required, the empty string counts once. */
    if (n == 0)
        return (seq_t)1;

    /* One‑time static allocation of two DP rows.  The routine
       seq_alloc_array constructs the objects properly for any seq_t.   */
    static seq_t *dp_prev = NULL;
    static seq_t *dp_cur  = NULL;
    static bool init = false;

    if (!init) {
        dp_prev = seq_alloc_array(19);
        dp_cur  = seq_alloc_array(19);
        if (!dp_prev || !dp_cur)
            abort();                     /* allocation failure */
        init = true;
    }

    /* The initial row (length 1) contains exactly one sequence ending
       with every possible digit.  Store it in dp_prev and compute the
       total number of length‑1 sequences. */
    seq_t total = 0;
    for (int d = 0; d < 19; ++d) {
        dp_prev[d] = (seq_t)1;
        total += (seq_t)1;                /* total = 19 */
    }

    /* For lengths 2 … n build successive rows of the DP table. */
    for (idx_t len = 2; len <= n; ++len) {
        /* For each digit d compute the number of sequences of
           current length that end with d.  Allowed previous digits
           are those in the interval [d-5, d+5] clipped to [0,18]. */
        for (int d = 0; d < 19; ++d) {
            idx_t low  = (d >= 5) ? (idx_t)(d - 5) : 0;
            idx_t high = (d + 5 < 19) ? (idx_t)(d + 5) : 18;
            seq_t s = 0;
            for (idx_t k = low; k <= high; ++k)
                s += dp_prev[k];
            dp_cur[d] = s;
        }

        /* Update the total number of length‑len sequences. */
        total = 0;
        for (int d = 0; d < 19; ++d)
            total += dp_cur[d];

        /* Rotate the two rows: the current row becomes the previous
           row for the next iteration. */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }

    return total;
}
