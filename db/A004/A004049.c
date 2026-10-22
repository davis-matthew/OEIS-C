/*
 * A004049  Erroneous version of A006967.
 * https://oeis.org/A004049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   search -- a(n) = number of graceful permutations of 1..n
 *            (permutations whose n-1 consecutive absolute differences are all
 *            distinct), as in A006967, counted by backtracking; this
 *            erroneous version has a(2) = 1 instead of 2
 * Range:     i8 n <= 9, i16 n <= 10, i32 n <= 10, i64 n <= 10, i128 n <= 10,
 *            big any n
 * Helpers:   from_i128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/from_i128.h"

static int g_n;
static bool *g_usedv;
static bool *g_usedd;
static unsigned long long g_cnt;

static void a004049_rec(int last, int depth)
{
    int v;
    if (depth == g_n) {
        g_cnt++;
        return;
    }
    for (v = 1; v <= g_n; v++) {
        int d;
        if (g_usedv[v]) continue;
        d = v > last ? v - last : last - v;
        if (g_usedd[d]) continue;
        g_usedv[v] = true;
        g_usedd[d] = true;
        a004049_rec(v, depth + 1);
        g_usedv[v] = false;
        g_usedd[d] = false;
    }
}

seq_t a(idx_t n)
{
    int s;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1;
    g_n = (int)n;
    g_usedv = (bool *)calloc((size_t)n + 1, sizeof(bool));
    g_usedd = (bool *)calloc((size_t)n + 1, sizeof(bool));
    g_cnt = 0;
    for (s = 1; s <= g_n; s++) {
        g_usedv[s] = true;
        a004049_rec(s, 1);
        g_usedv[s] = false;
    }
    free(g_usedv);
    free(g_usedd);
    return from_i128((__int128)g_cnt);
}
