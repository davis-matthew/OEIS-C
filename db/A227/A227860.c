/*
 * A227860  Sequence of integers such that there are d terms between pairs of
 *          integers d. Cycle through d=0,1,2,3, trying to insert the least
 *          unused pair starting at the next free position.
 * https://oeis.org/A227860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-8) for n > 10
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* initial terms: a(1)=0,a(2)=0,a(3)=1,a(4)=2,a(5)=1,a(6)=3,
   a(7)=2,a(8)=0,a(9)=0,a(10)=3 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t first10[11];
static bool first10_inited = false;

static void init_first10(void) {
    const idx_t limit = 10;
    /* allocate arrays for the first 10 positions (+ extra for safety) */
    seq_t *seq = (seq_t *)malloc((limit + 5) * sizeof(seq_t));
    bool *used = (bool *)calloc((limit + 5), sizeof(bool));
    int cycle[4] = {0, 1, 2, 3};
    idx_t idx = 1;          /* next free index */
    idx_t cyc = 0;          /* cycle index */

    while (idx <= limit) {
        int d = cycle[cyc];
        idx_t i = idx;
        while (i <= limit) {
            idx_t j = i + (idx_t)d + 1;
            bool j_ok = (j > limit) || !used[j];
            if (!used[i] && j_ok) {
                seq[i] = (seq_t)d;
                used[i] = true;
                if (j <= limit) {
                    seq[j] = (seq_t)d;
                    used[j] = true;
                }
                /* find next free position after i */
                idx_t next = i + 1;
                while (next <= limit && used[next]) {
                    next++;
                }
                idx = next;
                break;
            }
            i++;
        }
        cyc = (cyc + 1) & 3;   /* cycle through 0,1,2,3 */
    }

    for (idx_t i = 1; i <= limit; ++i) {
        first10[i] = seq[i];
    }
    free(seq);
    free(used);
    first10_inited = true;
}

seq_t a(idx_t n) {
    if (!first10_inited) {
        init_first10();
    }
    if (n <= 10) {
        return first10[n];
    }
    /* Reduce modulo the period 8 */
    while (n > 10) {
        n -= 8;
    }
    return first10[n];
}
