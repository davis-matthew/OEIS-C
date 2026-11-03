/*
 * A003870  Degrees of irreducible representations of symmetric group S_6.
 * https://oeis.org/A003870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = n-th smallest of the degrees 6!/prod(hook lengths) over
 *            all partitions of 6 (enumerated, then sorted)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: full, pass, 11 terms through n = 11 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int cmp_int(const void *x, const void *y) {
    int u = *(const int *)x, v = *(const int *)y;
    return (u > v) - (u < v);
}

static int hook_degree(const int *parts, int len) {
    long long prod = 1;
    for (int i = 0; i < len; i++) {
        for (int j = 0; j < parts[i]; j++) {
            int arm = parts[i] - j - 1;
            int leg = 0;
            for (int k = i + 1; k < len; k++)
                if (parts[k] > j) leg++;
            prod *= arm + leg + 1;
        }
    }
    long long f = 1;
    int total = 0;
    for (int i = 0; i < len; i++) total += parts[i];
    for (int i = 2; i <= total; i++) f *= i;
    return (int)(f / prod);
}

static int deg_list[64];
static int deg_count = 0;

static void gen(int *parts, int len, int remaining, int maxpart) {
    if (remaining == 0) {
        deg_list[deg_count++] = hook_degree(parts, len);
        return;
    }
    for (int p = (remaining < maxpart ? remaining : maxpart); p >= 1; p--) {
        parts[len] = p;
        gen(parts, len + 1, remaining - p, p);
    }
}

seq_t a(idx_t n) {
    if (deg_count == 0) {
        int parts[8];
        gen(parts, 0, 6, 6);
        qsort(deg_list, (size_t)deg_count, sizeof(int), cmp_int);
    }
    if (n < 1 || n > (idx_t)deg_count) return 0;
    return (seq_t)deg_list[(int)(n - 1)];
}
