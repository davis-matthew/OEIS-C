/*
 * A003874  Degrees of irreducible representations of symmetric group S_10.
 * https://oeis.org/A003874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest (with multiplicity) of 10!/prod(hook
 *            lengths) over all 42 partitions of 10 (hook length formula)
 * Range:     u8 n <= 26, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static unsigned long long degs[64];
static int ndegs = 0;

static unsigned long long hook_degree(const int *p, int len) {
    unsigned long long hp = 1;
    int i, j, k;
    for (i = 0; i < len; i++) {
        for (j = 0; j < p[i]; j++) {
            int leg = 0;
            for (k = i + 1; k < len; k++)
                if (p[k] > j) leg++;
            hp *= (unsigned long long)(p[i] - j + leg);
        }
    }
    return 3628800ULL / hp;
}

static void gen_parts(int *p, int len, int remaining, int maxpart) {
    int x;
    if (remaining == 0) {
        degs[ndegs++] = hook_degree(p, len);
        return;
    }
    for (x = (remaining < maxpart ? remaining : maxpart); x >= 1; x--) {
        p[len] = x;
        gen_parts(p, len + 1, remaining - x, x);
    }
}

seq_t a(idx_t n) {
    static bool computed = false;
    if (!computed) {
        int p[10];
        int i, j;
        ndegs = 0;
        gen_parts(p, 0, 10, 10);
        for (i = 1; i < ndegs; i++) {
            unsigned long long v = degs[i];
            j = i - 1;
            while (j >= 0 && degs[j] > v) {
                degs[j + 1] = degs[j];
                j--;
            }
            degs[j + 1] = v;
        }
        computed = true;
    }
    if (n < 1 || n > (idx_t)ndegs) return 0;
    return (seq_t)(idx_t)degs[(long long)(n - 1)];
}
