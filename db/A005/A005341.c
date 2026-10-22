/*
 * A005341  Length of n-th term in Look and Say sequences A005150 and A007651.
 * https://oeis.org/A005341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   a(n) = A055642(A005150(n)); computed with Conway's cosmological
 *            decomposition: the look-and-say string is split into
 *            non-interacting atoms (found by simulation from "1"), then a(n)
 *            = Sum_atoms count_n(atom)*length(atom), with counts evolved by
 *            the atom decay map
 * Range:     u8 n <= 19, u16 n <= 40, u32 n <= 81, u64 n <= 165,
 *            u128 n <= 332, big any n
 * Helpers:   alloc_zero, grow_zero, look_say
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/look_say.h"

#define LS_CAP 120
#define LS_K 40

static char **at_s = NULL;
static int *at_len = NULL;
static int **tr = NULL;
static int *ntr = NULL;
static int nat = 0, capat = 0;

static int find_or_add(const char *s, int len) {
    for (int i = 0; i < nat; i++)
        if (at_len[i] == len && memcmp(at_s[i], s, (size_t)len) == 0) return i;
    if (nat == capat) {
        capat = capat ? 2 * capat : 64;
        at_s = (char **)realloc(at_s, sizeof(char *) * (size_t)capat);
        at_len = (int *)realloc(at_len, sizeof(int) * (size_t)capat);
        tr = (int **)realloc(tr, sizeof(int *) * (size_t)capat);
        ntr = (int *)realloc(ntr, sizeof(int) * (size_t)capat);
    }
    at_s[nat] = (char *)malloc((size_t)len + 1);
    memcpy(at_s[nat], s, (size_t)len);
    at_s[nat][len] = 0;
    at_len[nat] = len;
    tr[nat] = NULL;
    ntr[nat] = 0;
    return nat++;
}

/* Can s be split as s[0..i) | s[i..len)?  True iff the first character of every
   descendant of the right part differs from the (constant) last char of the left part. */
static bool can_split(const char *s, int len, int i) {
    char c = s[i - 1];
    const char *R = s + i;
    int rlen = len - i;
    char p[2 * LS_CAP + 8], q[4 * LS_CAP + 16];
    int pl = rlen < LS_CAP ? rlen : LS_CAP;
    bool complete = rlen <= LS_CAP;
    memcpy(p, R, (size_t)pl);
    for (int k = 0; k < LS_K; k++) {
        if (pl == 0) return false;
        if (p[0] == c) return false;
        int ql = 0, j = 0;
        while (j < pl) {
            int r = 1;
            while (j + r < pl && p[j + r] == p[j]) r++;
            if (j + r == pl && !complete) break;
            if (r > 9) return false;
            q[ql++] = (char)('0' + r);
            q[ql++] = p[j];
            j += r;
        }
        if (ql > LS_CAP) { ql = LS_CAP; complete = false; }
        memcpy(p, q, (size_t)ql);
        pl = ql;
    }
    return true;
}

static int decompose(const char *s, int len, int *ids) {
    int n = 0, start = 0;
    for (int i = 1; i < len; i++) {
        if (can_split(s, len, i)) {
            ids[n++] = find_or_add(s + start, i - start);
            start = i;
        }
    }
    ids[n++] = find_or_add(s + start, len - start);
    return n;
}

static void build_atoms(void) {
    find_or_add("1", 1);
    for (int q = 0; q < nat; q++) {
        int len = at_len[q];
        char *src = (char *)malloc((size_t)len + 1);
        memcpy(src, at_s[q], (size_t)len);
        char *out = (char *)malloc((size_t)(4 * len + 32));
        int ol = look_say(src, len, out);
        int *ids = (int *)malloc(sizeof(int) * (size_t)(ol + 1));
        int m = decompose(out, ol, ids);
        tr[q] = ids;
        ntr[q] = m;
        free(out);
        free(src);
    }
}

static seq_t *vals = NULL;
static idx_t vals_cap = 0;
static idx_t computed = 0;
static seq_t *cnt = NULL;
static seq_t *nxt = NULL;

static seq_t total_len(void) {
    seq_t s = 0;
    for (int i = 0; i < nat; i++)
        if (cnt[i] != 0) s += cnt[i] * (seq_t)at_len[i];
    return s;
}

seq_t a(idx_t n) {
    if (computed == 0) {
        build_atoms();
        cnt = alloc_zero((idx_t)nat);
        nxt = alloc_zero((idx_t)nat);
        cnt[0] = 1;
        vals_cap = 64;
        vals = alloc_zero(vals_cap);
        vals[1] = total_len();
        computed = 1;
    }
    if (n < 1) n = 1;
    if (n >= vals_cap) {
        idx_t nc = vals_cap;
        while (nc <= n) nc *= 2;
        vals = grow_zero(vals, vals_cap, nc);
        vals_cap = nc;
    }
    while (computed < n) {
        for (int j = 0; j < nat; j++) nxt[j] = 0;
        for (int i = 0; i < nat; i++) {
            if (cnt[i] == 0) continue;
            for (int t = 0; t < ntr[i]; t++) nxt[tr[i][t]] += cnt[i];
        }
        seq_t *tmp = cnt;
        cnt = nxt;
        nxt = tmp;
        computed++;
        vals[computed] = total_len();
    }
    return vals[n];
}
