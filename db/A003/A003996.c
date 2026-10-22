/*
 * A003996  Sums of distinct nonzero squares in more than one way.
 * https://oeis.org/A003996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- count the ways each s <= 1000 is a sum of distinct
 *            nonzero squares with a 0/1 knapsack, capping counts at 2, and
 *            list every s that has 2 or more ways; past that range, a(n) = n
 *            + 68 (largest non-term is 132)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

#define A3996_B 1000

static int a3996_terms[A3996_B + 1];
static int a3996_cnt_terms = 0;
static bool a3996_done = false;

static void a3996_init(void) {
    int cnt[A3996_B + 1];
    int s, k;
    for (s = 0; s <= A3996_B; s++) cnt[s] = 0;
    cnt[0] = 1;
    for (k = 1; k * k <= A3996_B; k++) {
        int q = k * k;
        for (s = A3996_B; s >= q; s--) {
            int v = cnt[s] + cnt[s - q];
            cnt[s] = v > 2 ? 2 : v;
        }
    }
    a3996_cnt_terms = 0;
    for (s = 1; s <= A3996_B; s++)
        if (cnt[s] >= 2) a3996_terms[a3996_cnt_terms++] = s;
    a3996_done = true;
}

seq_t a(idx_t n) {
    if (!a3996_done) a3996_init();
    if (n >= 1 && n <= (idx_t)a3996_cnt_terms)
        return (seq_t)a3996_terms[n - 1];
    return (seq_t)n + 68;
}
