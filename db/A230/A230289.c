/*
 * A230289  Let m = n-th number not divisible by 3 (A001651); a(n) = position
 *          of m in A065075, or -1 if never appears in A065075.
 * https://oeis.org/A230289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search, not a closed form. A065075(1)=1 and A065075(j+1) =
 *            digsum(S_j), where S_1 = 1 and S_{j+1} = S_j + digsum(S_j). With
 *            m = floor((3n-1)/2) (A001651), a(n) = 1 if m = 1, otherwise j+1
 *            for the least j with digsum(S_j) = m. The S-iteration is walked
 *            with memoized block jumps: for each (level k,...
 * Range:     u8 n <= 16, u16 n <= 34, u32 n <= 64, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: standard, pass, 61 terms through n = 86 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define A230289_MW 6          /* mask words: digit sums < 384 */
#define A230289_BASE 3        /* simulate directly below 10^3 */
#define A230289_TOP 18        /* top-level block: 10^18 */

typedef struct {
    uint64_t steps;
    uint64_t rem;
    uint64_t mask[A230289_MW];
} A230289_res;

typedef struct {
    uint64_t k1, k2;          /* k2 == 0 means empty */
    A230289_res r;
} A230289_ent;

static uint64_t A230289_P10[20];
static A230289_ent *A230289_tab = NULL;
static size_t A230289_cap = 0, A230289_cnt = 0;

static unsigned A230289_ds(uint64_t x) {
    unsigned s = 0;
    while (x) { s += (unsigned)(x % 10); x /= 10; }
    return s;
}

static size_t A230289_hash(uint64_t k1, uint64_t k2) {
    uint64_t h = k1 * 0x9E3779B97F4A7C15ULL ^ (k2 + 0x632BE59BD9B4E019ULL);
    h ^= h >> 31; h *= 0xBF58476D1CE4E5B9ULL; h ^= h >> 29;
    return (size_t)h;
}

static void A230289_insert_raw(A230289_ent *t, size_t cap, const A230289_ent *e) {
    size_t i = A230289_hash(e->k1, e->k2) & (cap - 1);
    while (t[i].k2 != 0) i = (i + 1) & (cap - 1);
    t[i] = *e;
}

static void A230289_grow(void) {
    size_t ncap = A230289_cap ? A230289_cap * 2 : (size_t)1 << 16;
    A230289_ent *nt = (A230289_ent *)calloc(ncap, sizeof(A230289_ent));
    size_t i;
    for (i = 0; i < A230289_cap; i++)
        if (A230289_tab[i].k2 != 0) A230289_insert_raw(nt, ncap, &A230289_tab[i]);
    free(A230289_tab);
    A230289_tab = nt;
    A230289_cap = ncap;
}

static bool A230289_find(uint64_t k1, uint64_t k2, A230289_res *out) {
    size_t i;
    if (A230289_cap == 0) return false;
    i = A230289_hash(k1, k2) & (A230289_cap - 1);
    while (A230289_tab[i].k2 != 0) {
        if (A230289_tab[i].k1 == k1 && A230289_tab[i].k2 == k2) { *out = A230289_tab[i].r; return true; }
        i = (i + 1) & (A230289_cap - 1);
    }
    return false;
}

static void A230289_store(uint64_t k1, uint64_t k2, const A230289_res *r) {
    A230289_ent e;
    if ((A230289_cnt + 1) * 2 > A230289_cap) A230289_grow();
    e.k1 = k1; e.k2 = k2; e.r = *r;
    A230289_insert_raw(A230289_tab, A230289_cap, &e);
    A230289_cnt++;
}

static void A230289_setbit(uint64_t *mask, unsigned s) {
    if (s < 64u * A230289_MW) mask[s >> 6] |= (uint64_t)1 << (s & 63);
}

static bool A230289_hasbit(const uint64_t *mask, unsigned s) {
    if (s >= 64u * A230289_MW) return false;
    return (mask[s >> 6] >> (s & 63)) & 1u;
}

/* Walk states low (< 10^k after start digit) with extra digit sum c until value >= 10^k. */
static A230289_res A230289_jump(int k, unsigned c, uint64_t low) {
    A230289_res res;
    int w;
    res.steps = 0; res.rem = 0;
    for (w = 0; w < A230289_MW; w++) res.mask[w] = 0;
    if (k <= A230289_BASE) {
        uint64_t lim = A230289_P10[k];
        while (low < lim) {
            unsigned s = c + A230289_ds(low);
            A230289_setbit(res.mask, s);
            res.steps++;
            low += s;
        }
        res.rem = low - lim;
        return res;
    }
    {
        uint64_t k2 = ((uint64_t)k << 32) | ((uint64_t)c + 1);
        uint64_t q = A230289_P10[k - 1];
        uint64_t d = low / q, r = low % q;
        if (A230289_find(low, k2, &res)) return res;
        while (d < 10) {
            A230289_res sub = A230289_jump(k - 1, c + (unsigned)d, r);
            for (w = 0; w < A230289_MW; w++) res.mask[w] |= sub.mask[w];
            res.steps += sub.steps;
            d += 1 + sub.rem / q;
            r = sub.rem % q;
        }
        res.rem = (d - 10) * q + r;
        A230289_store(low, k2, &res);
        return res;
    }
}

/* Offset (0-based) of first state with digit sum m inside jump(k,c,low); must exist. */
static uint64_t A230289_locate(int k, unsigned c, uint64_t low, unsigned m) {
    uint64_t acc = 0;
    if (k <= A230289_BASE) {
        for (;;) {
            unsigned s = c + A230289_ds(low);
            if (s == m) return acc;
            acc++;
            low += s;
        }
    }
    {
        uint64_t q = A230289_P10[k - 1];
        uint64_t d = low / q, r = low % q;
        while (d < 10) {
            A230289_res sub = A230289_jump(k - 1, c + (unsigned)d, r);
            if (A230289_hasbit(sub.mask, m))
                return acc + A230289_locate(k - 1, c + (unsigned)d, r, m);
            acc += sub.steps;
            d += 1 + sub.rem / q;
            r = sub.rem % q;
        }
        return acc; /* not reached */
    }
}

seq_t a(idx_t n) {
    seq_t mv = ((seq_t)n * 3 - 1) / 2;
    unsigned m;
    uint64_t high = 0, low = 1, pos = 0;
    int it;

    if (A230289_P10[0] == 0) {
        int i;
        A230289_P10[0] = 1;
        for (i = 1; i < 20; i++) A230289_P10[i] = A230289_P10[i - 1] * 10;
    }
    if (mv >= (seq_t)(64 * A230289_MW)) return (seq_t)0 - 1;
    m = (unsigned)(long long)mv;
    if (m == 1) return (seq_t)1;

    /* S = high*10^18 + low; pos = number of states S_1.. before current block */
    for (it = 0; it < 100000; it++) {
        unsigned h = A230289_ds(high);
        A230289_res r = A230289_jump(A230289_TOP, h, low);
        if (A230289_hasbit(r.mask, m)) {
            uint64_t off = A230289_locate(A230289_TOP, h, low, m);
            /* state index j = pos + off + 1; answer = j + 1 */
            return (seq_t)(pos + off) + 2;
        }
        pos += r.steps;
        high += 1 + r.rem / A230289_P10[A230289_TOP];
        low = r.rem % A230289_P10[A230289_TOP];
    }
    return (seq_t)0 - 1;
}
