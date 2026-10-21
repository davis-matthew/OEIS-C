/*
 * A220130  Number of tilings of an n X 5 rectangle using integer-sided
 *          rectangular tiles of area n.
 * https://oeis.org/A220130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of tilings of an n×5 rectangle using integer-sided
 *            rectangular tiles of area n. The sequence has period 60 beyond
 *            a(0)=1. The values for n=1..60 are obtained by exhaustive
 *            enumeration of all possible tilings. Then a(n) = 1 for n=0 and
 *            a(n) = period[(n-1)%60] for n≥1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

typedef struct { unsigned int w; unsigned int h; } Shape;

static void dfs_tile(unsigned int n, unsigned char *occ, const Shape *shapes,
                     unsigned int num, unsigned int depth, unsigned long long *cnt) {
    if (depth == 5) {
        (*cnt)++;
        return;
    }
    unsigned int pos = 0, total = n * 5;
    while (pos < total && occ[pos]) ++pos;
    if (pos == total) return;          /* should not occur if depth<5 */
    unsigned int row = pos / 5, col = pos % 5;
    for (unsigned int i = 0; i < num; ++i) {
        unsigned int w = shapes[i].w, h = shapes[i].h;
        if (row + h > n || col + w > 5) continue;
        bool fits = true;
        for (unsigned int rr = row; rr < row + h && fits; ++rr)
            for (unsigned int cc = col; cc < col + w; ++cc)
                if (occ[rr * 5 + cc]) { fits = false; break; }
        if (!fits) continue;
        for (unsigned int rr = row; rr < row + h; ++rr)
            for (unsigned int cc = col; cc < col + w; ++cc)
                occ[rr * 5 + cc] = 1;
        dfs_tile(n, occ, shapes, num, depth + 1, cnt);
        for (unsigned int rr = row; rr < row + h; ++rr)
            for (unsigned int cc = col; cc < col + w; ++cc)
                occ[rr * 5 + cc] = 0;
    }
}

seq_t a(idx_t n) {
    static seq_t period[60];
    static bool initialized = false;
    if (!initialized) {
        for (unsigned int nn = 1; nn <= 60; ++nn) {
            Shape shapes[5];
            unsigned int num = 0;
            for (unsigned int w = 1; w <= 5; ++w)
                if (nn % w == 0) {
                    shapes[num].w = w;
                    shapes[num].h = nn / w;
                    ++num;
                }
            unsigned char occ[300] = {0};
            unsigned long long cnt = 0;
            dfs_tile(nn, occ, shapes, num, 0, &cnt);
            period[nn - 1] = (seq_t)cnt;
        }
        initialized = true;
    }
    if (n == 0) return (seq_t)1;
    unsigned int r = (unsigned int)((n - 1) % 60);
    return period[r];
}
