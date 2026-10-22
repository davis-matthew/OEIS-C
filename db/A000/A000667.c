/*
 * A000667  Boustrophedon transform of all-1's sequence.
 * https://oeis.org/A000667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   Boustrophedon (Seidel-Entringer) triangle. T(0,0) = 1, row i
 *            starts with 1 and is filled alternately left-to-right (odd i)
 *            and right-to-left (even i), each entry = previous entry in the
 *            row + the adjacent entry of row i-1; a(n) = last entry filled in
 *            row n. Equivalently a(n) = Sum_{k=0..n} binomi...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define A667_MAX 10000

static seq_t a667_row[2][A667_MAX + 2];
static seq_t a667_res[A667_MAX + 1];
static idx_t a667_done = 0;
static int a667_cur = 0;
static bool a667_init = false;

seq_t a(idx_t n) {
    if (!a667_init) {
        a667_row[0][0] = 1;
        a667_res[0] = 1;
        a667_cur = 0;
        a667_done = 0;
        a667_init = true;
    }
    if (n > A667_MAX) n = A667_MAX;
    while (a667_done < n) {
        idx_t i = a667_done + 1, k;
        seq_t *prev = a667_row[a667_cur];
        seq_t *curr = a667_row[a667_cur ^ 1];
        if (i % 2 == 1) {
            curr[0] = 1;
            for (k = 1; k <= i; k++)
                curr[k] = curr[k - 1] + prev[k - 1];
            a667_res[i] = curr[i];
        } else {
            curr[i] = 1;
            for (k = i; k > 0; k--)
                curr[k - 1] = curr[k] + prev[k - 1];
            a667_res[i] = curr[0];
        }
        a667_cur ^= 1;
        a667_done = i;
    }
    return a667_res[n];
}
