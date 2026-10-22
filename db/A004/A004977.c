/*
 * A004977  Sum of digits of n-th term in Look and Say sequence A005150.
 * https://oeis.org/A004977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of digits of A005150(n). The look-and-say string is
 *            split into independently evolving atoms (Conway's cosmological
 *            decomposition, with the split points found by simulation). The
 *            code then counts atoms with the decay-matrix recurrence cnt_{n}
 *            = M * cnt_{n-1}, cnt_1 = [atom "1"], and a(n...
 * Range:     u8 n <= 17, u16 n <= 38, u32 n <= 79, u64 n <= 163,
 *            u128 n <= 330, big any n
 * Helpers:   alloc_zero, grow_zero, las
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/las.h"

#define MAXA 2048
#define SCAP 256
#define SK 40

static int natoms = 0;
static char *astr[MAXA];
static int alen[MAXA];
static int ads[MAXA];
static int nch[MAXA];
static int *chl[MAXA];

static int can_split(char d, const char *r, int rlen) {
    char b1[2 * SCAP + 8], b2[2 * SCAP + 8];
    char *cur = b1, *nx = b2, *t;
    int len = rlen < SCAP ? rlen : SCAP;
    int trunc = rlen > SCAP;
    int k, nl;
    memcpy(cur, r, (size_t)len);
    for (k = 0; k <= SK; k++) {
        if (len == 0) return 1;
        if (cur[0] == d) return 0;
        if (k == SK) break;
        nl = las(cur, len, nx, trunc);
        if (nl > SCAP) { nl = SCAP; trunc = 1; }
        t = cur; cur = nx; nx = t;
        len = nl;
    }
    return 1;
}

static int find_or_add(const char *s, int len) {
    int i, j, ds = 0;
    for (i = 0; i < natoms; i++)
        if (alen[i] == len && memcmp(astr[i], s, (size_t)len) == 0) return i;
    astr[natoms] = (char *)malloc((size_t)len + 1);
    memcpy(astr[natoms], s, (size_t)len);
    alen[natoms] = len;
    for (j = 0; j < len; j++) ds += s[j] - '0';
    ads[natoms] = ds;
    nch[natoms] = -1;
    return natoms++;
}

static void build_atoms(void) {
    int idx, i;
    find_or_add("1", 1);
    for (idx = 0; idx < natoms; idx++) {
        int len = alen[idx];
        char *nx = (char *)malloc((size_t)(2 * len + 2));
        int nl = las(astr[idx], len, nx, 0);
        int *tmp = (int *)malloc(sizeof(int) * (size_t)(nl + 1));
        int cnt = 0, start = 0;
        for (i = 1; i <= nl; i++) {
            if (i == nl || (nx[i] != nx[i - 1] && can_split(nx[i - 1], nx + i, nl - i))) {
                tmp[cnt++] = find_or_add(nx + start, i - start);
                start = i;
            }
        }
        nch[idx] = cnt;
        chl[idx] = tmp;
        free(nx);
    }
}

static seq_t *cntv = NULL, *tmpv = NULL, *res = NULL;
static idx_t done = 0, rescap = 0;

static seq_t eval_sum(void) {
    seq_t s = 0;
    int i;
    for (i = 0; i < natoms; i++)
        if (cntv[i] != 0) s += cntv[i] * (seq_t)ads[i];
    return s;
}

seq_t a(idx_t n) {
    int i, j;
    if (n < 1) return 0;
    if (natoms == 0) {
        build_atoms();
        cntv = alloc_zero((idx_t)natoms);
        tmpv = alloc_zero((idx_t)natoms);
        cntv[0] = 1;
        rescap = 64;
        res = alloc_zero(rescap + 1);
        res[1] = eval_sum();
        done = 1;
    }
    if (n > rescap) {
        idx_t nc = rescap;
        while (nc < n) nc *= 2;
        res = grow_zero(res, rescap + 1, nc + 1);
        rescap = nc;
    }
    while (done < n) {
        seq_t *t;
        for (j = 0; j < natoms; j++) tmpv[j] = 0;
        for (i = 0; i < natoms; i++) {
            if (cntv[i] == 0) continue;
            for (j = 0; j < nch[i]; j++) tmpv[chl[i][j]] += cntv[i];
        }
        t = cntv; cntv = tmpv; tmpv = t;
        done++;
        res[done] = eval_sum();
    }
    return res[n];
}
