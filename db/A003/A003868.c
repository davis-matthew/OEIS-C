/*
 * A003868  Degrees of irreducible representations of alternating group A_13.
 * https://oeis.org/A003868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   degrees of irreducible representations of A_13, sorted
 *            ascending. For each partition p of 13, f(p) = 13!/prod(hook
 *            lengths). A conjugate pair p != p' gives one irrep of degree
 *            f(p). A self-conjugate p gives two irreps, each of degree
 *            f(p)/2. The code enumerates partitions to find these
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

#define NN 13

static int part[NN + 1];
static seq_t degs[200];
static int ndegs = 0;

static uint64_t hook_degree(const int *p, int len) {
    uint64_t fact = 1, hp = 1;
    int i, j, k;
    for (i = 2; i <= NN; i++) fact *= (uint64_t)i;
    for (i = 0; i < len; i++) {
        for (j = 0; j < p[i]; j++) {
            int arm = p[i] - j - 1, leg = 0;
            for (k = i + 1; k < len; k++)
                if (p[k] > j) leg++;
            hp *= (uint64_t)(arm + leg + 1);
        }
    }
    return fact / hp;
}

/* returns >0 if p > p' lexicographically, 0 if equal, <0 otherwise */
static int cmp_conj(const int *p, int len) {
    int pa[NN], ca[NN];
    int i, j;
    for (i = 0; i < NN; i++) { pa[i] = (i < len) ? p[i] : 0; ca[i] = 0; }
    for (j = 0; j < NN; j++) {
        int c = 0;
        for (i = 0; i < len; i++)
            if (p[i] > j) c++;
        ca[j] = c;
    }
    for (i = 0; i < NN; i++) {
        if (pa[i] != ca[i]) return pa[i] - ca[i];
    }
    return 0;
}

static void gen(int remaining, int max_part, int depth) {
    int i;
    if (remaining == 0) {
        int c = cmp_conj(part, depth);
        if (c > 0) {
            degs[ndegs++] = (seq_t)hook_degree(part, depth);
        } else if (c == 0) {
            uint64_t h = hook_degree(part, depth) / 2;
            degs[ndegs++] = (seq_t)h;
            degs[ndegs++] = (seq_t)h;
        }
        return;
    }
    for (i = 1; i <= remaining && i <= max_part; i++) {
        part[depth] = i;
        gen(remaining - i, i, depth + 1);
    }
}

seq_t a(idx_t n) {
    static int computed = 0;
    if (!computed) {
        int i, j;
        ndegs = 0;
        gen(NN, NN, 0);
        /* insertion sort */
        for (i = 1; i < ndegs; i++) {
            seq_t v = degs[i];
            j = i - 1;
            while (j >= 0 && degs[j] > v) {
                degs[j + 1] = degs[j];
                j--;
            }
            degs[j + 1] = v;
        }
        computed = 1;
    }
    if (n >= 1 && n <= (idx_t)ndegs) return degs[n - 1];
    return (seq_t)0;
}
