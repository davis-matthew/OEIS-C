/*
 * A003187  Number of positive threshold functions of n variables.
 * https://oeis.org/A003187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of positive (monotone) threshold functions of n
 *            variables up to permutation of the variables, i.e. regular
 *            threshold functions with weights w1 >= w2 >= ... >= wn >= 0.
 *            Computed by a search: enumerate every sorted integer weight
 *            vector up to the known maximal minimal weight (1,1,2,3,...
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define A3187_HBITS 17
#define A3187_HSIZE (1u << A3187_HBITS)
#define A3187_HMASK (A3187_HSIZE - 1u)

static uint64_t a3187_hl[A3187_HSIZE], a3187_hh[A3187_HSIZE];
static unsigned char a3187_hu[A3187_HSIZE];
static long long a3187_cnt;
static int a3187_n;
static int a3187_sums[9][256];

static void a3187_ins(uint64_t lo, uint64_t hi)
{
    uint64_t h = lo * 0x9E3779B97F4A7C15ULL ^ (hi + 0x632BE59BD9B4E019ULL) * 0xC2B2AE3D27D4EB4FULL;
    h ^= h >> 29;
    size_t i = (size_t)(h & A3187_HMASK);
    while (a3187_hu[i]) {
        if (a3187_hl[i] == lo && a3187_hh[i] == hi) return;
        i = (i + 1) & A3187_HMASK;
    }
    a3187_hu[i] = 1;
    a3187_hl[i] = lo;
    a3187_hh[i] = hi;
    a3187_cnt++;
}

static void a3187_leaf(void)
{
    int N = 1 << a3187_n;
    const int *s = a3187_sums[a3187_n];
    int maxs = s[N - 1];
    uint64_t ml[260], mh[260];
    memset(ml, 0, sizeof(uint64_t) * (size_t)(maxs + 1));
    memset(mh, 0, sizeof(uint64_t) * (size_t)(maxs + 1));
    for (int x = 0; x < N; x++) {
        int v = s[x];
        if (x < 64) ml[v] |= 1ULL << x;
        else mh[v] |= 1ULL << (x - 64);
    }
    uint64_t lo = 0, hi = 0;
    for (int v = maxs; v >= 0; v--) {
        if (ml[v] | mh[v]) {
            lo |= ml[v];
            hi |= mh[v];
            a3187_ins(lo, hi);
        }
    }
}

static void a3187_rec(int k, int maxw)
{
    if (k == a3187_n) {
        a3187_leaf();
        return;
    }
    int half = 1 << k;
    for (int w = 0; w <= maxw; w++) {
        for (int x = 0; x < half; x++) {
            a3187_sums[k + 1][x] = a3187_sums[k][x];
            a3187_sums[k + 1][x + half] = a3187_sums[k][x] + w;
        }
        a3187_rec(k + 1, w);
    }
}

seq_t a(idx_t n)
{
    static long long memo[8];
    static bool done[8];
    static const int maxw[8] = {0, 2, 3, 4, 6, 9, 12, 18};
    if (n < 1 || n > 7) return 0;
    int k = (int)n;
    if (!done[k]) {
        memset(a3187_hu, 0, sizeof(a3187_hu));
        a3187_cnt = 0;
        a3187_n = k;
        a3187_sums[0][0] = 0;
        a3187_ins(0, 0); /* constant 0 */
        a3187_rec(0, maxw[k]);
        memo[k] = a3187_cnt;
        done[k] = true;
    }
    return (seq_t)memo[k];
}
