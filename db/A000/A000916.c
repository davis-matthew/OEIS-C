/*
 * A000916  a(2n) = n+2, a(2n-1) = smallest number requiring n+2 letters in
 *          English.
 * https://oeis.org/A000916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   a(2n) = n+2; a(2n-1) = smallest number whose British English
 *            name (with "and" before the final part below 100, e.g. "one
 *            thousand one hundred and four", "three thousand and
 *            seventy-three") has n+2 letters. It is found by a greedy
 *            digit-group search: use the fewest 3-digit groups that can reach
 *            th...
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

#define A916_NG 22
#define A916_MAXS 1200

static const int a916_ones[10] = {0, 3, 3, 5, 4, 4, 3, 5, 5, 4};
static const int a916_teens[10] = {3, 6, 6, 8, 8, 7, 7, 9, 8, 8};
static const int a916_tens[10] = {0, 0, 6, 6, 5, 5, 5, 7, 6, 6};
/* thousand, million, billion, trillion, quadrillion, ... vigintillion */
static const int a916_scale[A916_NG] = {0, 8, 7, 7, 8, 11, 11, 10, 10, 9, 9, 9,
                                        11, 12, 12, 17, 13, 12, 15, 13, 14, 12};

static int a916_glen[1000];
static char a916_R[A916_NG + 1][A916_MAXS];
static int a916_init = 0;

static int a916_group(int v) {
    int h = v / 100, r = v % 100, len = 0;
    if (h) len += a916_ones[h] + 7 + (r ? 3 : 0);
    if (r < 10) len += a916_ones[r];
    else if (r < 20) len += a916_teens[r - 10];
    else len += a916_tens[r / 10] + a916_ones[r % 10];
    return len;
}

/* letters contributed by group j holding v, in a number of at least 2 groups */
static int a916_contrib(int j, int v) {
    if (v == 0) return 0;
    if (j == 0) return a916_glen[v] + (v < 100 ? 3 : 0);
    return a916_glen[v] + a916_scale[j];
}

static void a916_setup(void) {
    int v, j, s;
    for (v = 0; v < 1000; v++) a916_glen[v] = a916_group(v);
    memset(a916_R, 0, sizeof(a916_R));
    a916_R[0][0] = 1;
    for (j = 0; j < A916_NG; j++)
        for (s = 0; s < A916_MAXS; s++)
            if (a916_R[j][s])
                for (v = 0; v < 1000; v++) {
                    int t = s + a916_contrib(j, v);
                    if (t < A916_MAXS) a916_R[j + 1][t] = 1;
                }
    a916_init = 1;
}

static seq_t a916_smallest(int L) {
    int G, j, v;
    if (!a916_init) a916_setup();
    for (v = 1; v < 1000; v++)
        if (a916_glen[v] == L) return (seq_t)v;
    for (G = 2; G <= A916_NG; G++) {
        int rem = L, ok = 0;
        seq_t res = 0;
        for (v = 1; v < 1000; v++) {
            int t = L - a916_contrib(G - 1, v);
            if (t >= 0 && t < A916_MAXS && a916_R[G - 1][t]) { ok = 1; break; }
        }
        if (!ok) continue;
        for (j = G - 1; j >= 0; j--) {
            for (v = (j == G - 1) ? 1 : 0; v < 1000; v++) {
                int t = rem - a916_contrib(j, v);
                if (t >= 0 && t < A916_MAXS && a916_R[j][t]) break;
            }
            rem -= a916_contrib(j, v);
            res = res * 1000 + (seq_t)v;
        }
        return res;
    }
    return 0;
}

seq_t a(idx_t n) {
    if (n % 2 == 0) return (seq_t)(n / 2) + 2;
    {
        idx_t k = (n + 1) / 2;
        if (k + 2 >= A916_MAXS) return 0;
        return a916_smallest((int)k + 2);
    }
}
