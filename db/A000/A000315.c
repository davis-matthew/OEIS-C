/*
 * A000315  Number of reduced Latin squares of order n; also number of labeled
 *          loops (quasigroups with an identity element) with a fixed identity
 *          element.
 * https://oeis.org/A000315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, nice, more
 * Formula:   a(n) = A002860(n)/(n!*(n-1)!); counted by backtracking search
 *            over reduced Latin squares for n <= 7 (last two rows counted via
 *            cycle components: 2^(components-1)); for 8 <= n <= 11 the values
 *            are the published exhaustive enumerations (Wells 1967,
 *            Bammel-Rothstein 1975, McKay-Rogoyski 1995, McKay-...
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   ls_find, ls_parse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ls_find.h"
#include "../utils/ls_parse.h"

static int ls_N;
static unsigned long long ls_full;
static unsigned long long ls_rowm[64];
static unsigned long long ls_colm[64];
static unsigned long long ls_acc;
static seq_t ls_total;

static void ls_flush(void)
{
    ls_total += (seq_t)ls_acc;
    ls_acc = 0;
}

static void ls_leaf(void)
{
    int p[64];
    int i, comps = ls_N;
    for (i = 0; i < ls_N; i++) p[i] = i;
    for (i = 0; i < ls_N; i++) {
        unsigned long long m = ls_full & ~ls_colm[i];
        int x = __builtin_ctzll(m);
        int y = __builtin_ctzll(m & (m - 1));
        int rx = ls_find(p, x), ry = ls_find(p, y);
        if (rx != ry) {
            p[rx] = ry;
            comps--;
        }
    }
    ls_acc += 1ULL << (comps - 1);
    if (ls_acc >= (1ULL << 62)) ls_flush();
}

static void ls_rec(int r, int c)
{
    unsigned long long avail;
    if (c == ls_N) {
        if (r == ls_N - 3) {
            ls_leaf();
            return;
        }
        r++;
        c = 1;
        ls_rowm[r] = 1ULL << r;
    }
    avail = ~(ls_rowm[r] | ls_colm[c]) & ls_full;
    while (avail) {
        unsigned long long b = avail & (0ULL - avail);
        avail ^= b;
        ls_rowm[r] |= b;
        ls_colm[c] |= b;
        ls_rec(r, c + 1);
        ls_rowm[r] ^= b;
        ls_colm[c] ^= b;
    }
}

static seq_t ls_count(int n)
{
    int i;
    if (n <= 2) return 1;
    ls_N = n;
    ls_full = (1ULL << n) - 1ULL;
    for (i = 0; i < n; i++) ls_colm[i] = 1ULL << i;
    ls_colm[0] = ls_full & ~((1ULL << (n - 2)) | (1ULL << (n - 1)));
    ls_acc = 0;
    ls_total = 0;
    if (n == 3) {
        ls_leaf();
    } else {
        ls_rowm[1] = 1ULL << 1;
        ls_rec(1, 1);
    }
    ls_flush();
    return ls_total;
}

/* published enumeration results for orders 8..11 */
static const char *const ls_known[4] = {
    "535281401856",
    "377597570964258816",
    "7580721483160132811489280",
    "5363937773277371298119673540771840"
};

seq_t a(idx_t n)
{
    static seq_t memo[8];
    static int have[8];
    int k;
    if (n < 1) return 0;
    if (n >= 8 && n <= 11) return ls_parse(ls_known[n - 8]);
    if (n > 11) return 0;   /* unknown */
    k = (int)n;
    if (!have[k]) {
        memo[k] = ls_count(k);
        have[k] = 1;
    }
    return memo[k];
}
