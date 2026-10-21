/*
 * A202935  Number of (n+3) X 7 binary arrays with consecutive windows of four
 *          bits considered as a binary number nondecreasing in every row and
 *          column.
 * https://oeis.org/A202935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) for n > 8 Initial values:
 *            a(1)=130321, a(2)=206145, a(3)=330853, a(4)=533832, a(5)=857408,
 *            a(6)=1360328, a(7)=2121734, a(8)=3245653 (Index starts at 1)
 * Range:     u8 none, u16 none, u32 n <= 38, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[8] = {
    (seq_t)130321,
    (seq_t)206145,
    (seq_t)330853,
    (seq_t)533832,
    (seq_t)857408,
    (seq_t)1360328,
    (seq_t)2121734,
    (seq_t)3245653
};

static seq_t *seq_cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Not part of the OEIS offset, but safe */

    if (cache_len < n) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_len * sizeof(seq_t));
        if (!tmp) exit(1);  /* Allocation failure; abort */
        seq_cache = tmp;

        while (cache_len < new_len) {
            if (cache_len < 8) {
                seq_cache[cache_len] = init[cache_len];
            } else {
                seq_t a1 = seq_cache[cache_len-1];
                seq_t a2 = seq_cache[cache_len-2];
                seq_t a3 = seq_cache[cache_len-3];
                seq_t a4 = seq_cache[cache_len-4];
                seq_t a5 = seq_cache[cache_len-5];
                seq_t a6 = seq_cache[cache_len-6];
                seq_t a7 = seq_cache[cache_len-7];
                seq_t a8 = seq_cache[cache_len-8];
                seq_cache[cache_len] =
                    8*a1 - 28*a2 + 56*a3 - 70*a4 + 56*a5 - 28*a6 + 8*a7 - a8;
            }
            cache_len++;
        }
    }
    return seq_cache[n-1];
}
