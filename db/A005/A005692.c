/*
 * A005692  Shortest wins at Beanstalk.
 * https://oeis.org/A005692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Beanstalk game: from an even number
 *            the move is forced to n/2; from an odd n>1 the player picks 3n-1
 *            or 3n+1. The player who reaches 1 wins. a(n) is the length of
 *            the win from 2n+1 when the winner plays to win as fast as
 *            possible and the loser to lose as slowly as possi...
 * Range:     u8 n <= 372, u16 n <= 999, u32 n <= 999, u64 n <= 999,
 *            u128 n <= 999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef unsigned __int128 bs_u128;

struct bs_ent { bs_u128 key; int wT, wF, lT, lF; };

static struct bs_ent *bs_tab = NULL;
static size_t bs_cap = 0, bs_cnt = 0;

static int bs_bitlen(bs_u128 x) {
    uint64_t hi = (uint64_t)(x >> 64), lo = (uint64_t)x;
    if (hi) return 128 - __builtin_clzll(hi);
    return lo ? 64 - __builtin_clzll(lo) : 0;
}

static int bs_ctz(bs_u128 y) {
    uint64_t lo = (uint64_t)y;
    if (lo) return __builtin_ctzll(lo);
    return 64 + __builtin_ctzll((uint64_t)(y >> 64));
}

static size_t bs_hash(bs_u128 x) {
    uint64_t h = (uint64_t)x ^ ((uint64_t)(x >> 64) * 0xC2B2AE3D27D4EB4FULL);
    h *= 0x9E3779B97F4A7C15ULL;
    h ^= h >> 29;
    return (size_t)h;
}

static void bs_insert_raw(struct bs_ent *t, size_t cap, struct bs_ent e) {
    size_t i = bs_hash(e.key) & (cap - 1);
    while (t[i].key != 0) i = (i + 1) & (cap - 1);
    t[i] = e;
}

static void bs_grow(void) {
    size_t ncap = bs_cap ? bs_cap * 2 : (size_t)1 << 16;
    struct bs_ent *nt = (struct bs_ent *)calloc(ncap, sizeof(struct bs_ent));
    size_t i;
    for (i = 0; i < bs_cap; i++)
        if (bs_tab[i].key != 0) bs_insert_raw(nt, ncap, bs_tab[i]);
    free(bs_tab);
    bs_tab = nt;
    bs_cap = ncap;
}

static size_t bs_find(bs_u128 x) {
    size_t i;
    if ((bs_cnt + 1) * 2 >= bs_cap) bs_grow();
    i = bs_hash(x) & (bs_cap - 1);
    while (bs_tab[i].key != 0) {
        if (bs_tab[i].key == x) return i;
        i = (i + 1) & (bs_cap - 1);
    }
    bs_tab[i].key = x;
    bs_tab[i].wT = 1 << 30;
    bs_tab[i].wF = -1;
    bs_tab[i].lT = 1 << 30;
    bs_tab[i].lF = -1;
    bs_cnt++;
    return i;
}

static bool bs_win(bs_u128 x, int D);
static bool bs_lose(bs_u128 x, int D);

/* from odd x>1, option j: number of forced moves k, and next odd value */
static void bs_option(bs_u128 x, int j, int *k, bs_u128 *xp) {
    bs_u128 y = j ? 3 * x + 1 : 3 * x - 1;
    int v = bs_ctz(y);
    *k = 1 + v;
    *xp = y >> v;
}

static bool bs_win(bs_u128 x, int D) {
    size_t i;
    int j, k;
    bs_u128 xp;
    bool res = false;
    if (x == 1) return false;
    if (D < bs_bitlen(x) - 1) return false;
    i = bs_find(x);
    if (D >= bs_tab[i].wT) return true;
    if (D <= bs_tab[i].wF) return false;
    for (j = 0; j < 2 && !res; j++) {
        bs_option(x, j, &k, &xp);
        if (k > D) continue;
        if (k & 1) res = bs_lose(xp, D - k);
        else res = bs_win(xp, D - k);
    }
    i = bs_find(x);
    if (res) { if (D < bs_tab[i].wT) bs_tab[i].wT = D; }
    else { if (D > bs_tab[i].wF) bs_tab[i].wF = D; }
    return res;
}

static bool bs_lose(bs_u128 x, int D) {
    size_t i;
    int j, k;
    bs_u128 xp;
    bool res = true;
    if (x == 1) return true;
    if (D < bs_bitlen(x) - 1) return false;
    i = bs_find(x);
    if (D >= bs_tab[i].lT) return true;
    if (D <= bs_tab[i].lF) return false;
    for (j = 0; j < 2 && res; j++) {
        bs_option(x, j, &k, &xp);
        if (k > D) { res = false; break; }
        if (k & 1) res = bs_win(xp, D - k);
        else res = bs_lose(xp, D - k);
    }
    i = bs_find(x);
    if (res) { if (D < bs_tab[i].lT) bs_tab[i].lT = D; }
    else { if (D > bs_tab[i].lF) bs_tab[i].lF = D; }
    return res;
}

seq_t a(idx_t n) {
    bs_u128 x = (bs_u128)2 * (bs_u128)n + 1;
    int D;
    if (x == 1) return (seq_t)0;
    for (D = 1; ; D += 2)
        if (bs_win(x, D)) return (seq_t)D;
}
