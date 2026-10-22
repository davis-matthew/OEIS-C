/*
 * A001619  Number of letters in English name for n increases at these
 *          numbers.
 * https://oeis.org/A001619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  word, nonn
 * Formula:   search, not a closed form. Each term is the smallest number
 *            whose English name has more letters than the previous term's
 *            name. The name is built from groups of three digits (British
 *            "and" usage, "-" counted as a letter, short-scale names up to
 *            vigintillion), and the greedy step picks each group f...
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static const int A1619_ONES[20] = {0,3,3,5,4,4,3,5,5,4,3,6,6,8,8,7,7,9,8,8};
static const int A1619_TENS[10] = {0,0,6,6,5,5,5,7,6,6};
/* thousand, million, billion, ..., vigintillion */
static const int A1619_SCALE[22] = {0,8,7,7,8,11,11,10,10,9,9,9,11,12,12,17,13,12,15,13,14,12};
#define A1619_MAXG 22

static int a1619_f(int g) {
    int h = g / 100, r = g % 100, c = 0;
    if (h) c += A1619_ONES[h] + 7;          /* "x hundred" */
    if (h && r) c += 3;                     /* "and" */
    if (r < 20) c += A1619_ONES[r];
    else {
        c += A1619_TENS[r / 10];
        if (r % 10) c += 1 + A1619_ONES[r % 10];  /* hyphen + unit */
    }
    return c;
}

static int a1619_contrib(int i, int g, int k) {
    int c;
    if (g == 0) return 0;
    c = a1619_f(g);
    if (i > 0) c += A1619_SCALE[i];
    else if (k > 1 && g < 100) c += 3;      /* "one thousand and five" */
    return c;
}

/* smallest number with letter count >= R; returns false if beyond names */
static bool a1619_min_with(int R, int *grp, int *kout, int *Lout) {
    int maxc[A1619_MAXG];
    int k, i, g;
    for (k = 1; k <= A1619_MAXG; k++) {
        int tot = 0;
        for (i = 0; i < k; i++) {
            int m = 0;
            for (g = 0; g < 1000; g++) {
                int c = a1619_contrib(i, g, k);
                if (c > m) m = c;
            }
            maxc[i] = m;
            tot += m;
        }
        if (tot >= R) {
            int cur = 0;
            for (i = k - 1; i >= 0; i--) {
                int rest = 0, j;
                for (j = 0; j < i; j++) rest += maxc[j];
                for (g = (i == k - 1) ? 1 : 0; g < 1000; g++) {
                    int c = a1619_contrib(i, g, k);
                    if (cur + c + rest >= R) break;
                }
                grp[i] = g;
                cur += a1619_contrib(i, g, k);
            }
            *kout = k;
            *Lout = cur;
            return true;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    int grp[A1619_MAXG];
    int k = 0, L = 0, T = 0, i;
    idx_t c;
    seq_t v = 0;
    for (c = 0; c < n; c++) {
        if (!a1619_min_with(T + 1, grp, &k, &L)) return 0;
        T = L;
    }
    for (i = k - 1; i >= 0; i--) v = v * 1000 + (seq_t)grp[i];
    return v;
}
