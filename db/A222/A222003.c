/*
 * A222003  Number of nX5 arrays with each row a permutation of 1..5 having at
 *          least as many downsteps as the preceding row, with rows in
 *          lexicographically nonincreasing order.
 * https://oeis.org/A222003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 arrays whose rows are 5‑permutations, rows
 *            are lexicographically nonincreasing, and the number of downsteps
 *            (adjacent decreases) in each row is nondecreasing from row to
 *            row. Computed by dynamic programming over 120 states (all 5!
 *            permutations)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 20, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define NB 120   /* number of 5‑permutations */

/* helper: next lexicographic permutation (C++ style) */
static inline bool next_permutation(int *a, int sz) {
    int i = sz - 2;
    while (i >= 0 && a[i] > a[i + 1]) i--;
    if (i < 0) return false;
    int j = sz - 1;
    while (a[j] < a[i]) j--;
    int tmp = a[i]; a[i] = a[j]; a[j] = tmp;
    for (int l = i + 1, r = sz - 1; l < r; l++, r--) {
        tmp = a[l]; a[l] = a[r]; a[r] = tmp;
    }
    return true;
}

seq_t a(idx_t n) {
    /* static arrays for state transition */
    static int perm[NB][5];
    static int down[NB];
    static int edges[NB][NB];
    static int nbr[NB];
    static bool initialized = false;

    if (!initialized) {
        /* generate all 5‑permutations in lexicographic order */
        int base[5] = {1, 2, 3, 4, 5};
        int idx = 0;
        do {
            for (int i = 0; i < 5; i++) perm[idx][i] = base[i];
            int d = 0;
            for (int i = 0; i < 4; i++) if (perm[idx][i] > perm[idx][i + 1]) d++;
            down[idx] = d;
            idx++;
        } while (next_permutation(base, 5));

        /* build adjacency: from i to j iff lex(i) >= lex(j) and down(i) <= down(j) */
        for (int i = 0; i < NB; i++) {
            int cnt = 0;
            for (int j = 0; j <= i; j++) {
                if (down[j] >= down[i]) edges[i][cnt++] = j;
            }
            nbr[i] = cnt;
        }

        initialized = true;
    }

    /* dynamic programming */
    static seq_t dp_cur[NB];
    static seq_t dp_next[NB];
    for (int i = 0; i < NB; i++) dp_cur[i] = 1;   /* length 1 */

    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < NB; i++) dp_next[i] = 0;
        for (int i = 0; i < NB; i++) {
            if (dp_cur[i] == 0) continue;
            seq_t val = dp_cur[i];
            for (int k = 0; k < nbr[i]; k++) {
                int j = edges[i][k];
                dp_next[j] += val;
            }
        }
        for (int i = 0; i < NB; i++) dp_cur[i] = dp_next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < NB; i++) total += dp_cur[i];
    return total;
}
