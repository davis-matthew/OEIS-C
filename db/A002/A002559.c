/*
 * A002559  Markoff (or Markov) numbers: union of positive integers x, y, z
 *          satisfying x^2 + y^2 + z^2 = 3*x*y*z.
 * https://oeis.org/A002559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search, not a closed form. Walk the Markoff tree from (1,2,5):
 *            each sorted triple (x,y,z) has children (x,z,3xz-y) and
 *            (y,z,3yz-x). Collect every z up to a bound B, together with 1
 *            and 2. Sort and dedupe the values, then raise B until there are
 *            at least n terms
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 98, u64 n <= 376,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 965 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* computes 3*a*z - b if it is <= B, without overflow (requires b <= z <= B) */
static int mk_child(seq_t a, seq_t b, seq_t z, seq_t B, seq_t *out) {
    if (a > B / z) return 0;
    seq_t p = a * z;
    seq_t w = p - b;
    if (p > B - w) return 0;
    seq_t s = p + w;
    if (p > B - s) return 0;
    *out = s + p;
    return 1;
}

static seq_t *mk_grow_vals(seq_t *old, idx_t oldlen, idx_t newcap) {
    seq_t *p = mem_new(seq_t, (size_t)newcap);
    for (idx_t i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static void mk_sift(seq_t *v, idx_t i, idx_t len) {
    for (;;) {
        idx_t l = 2 * i + 1, m = i;
        if (l < len && v[l] > v[m]) m = l;
        if (l + 1 < len && v[l + 1] > v[m]) m = l + 1;
        if (m == i) return;
        seq_t t = v[i]; v[i] = v[m]; v[m] = t;
        i = m;
    }
}

static void mk_sort(seq_t *v, idx_t len) {
    if (len < 2) return;
    for (idx_t i = len / 2; i-- > 0;) mk_sift(v, i, len);
    for (idx_t e = len - 1; e > 0; e--) {
        seq_t t = v[0]; v[0] = v[e]; v[e] = t;
        mk_sift(v, 0, e);
    }
}

typedef struct { seq_t x, y, z; } mk_trip;

static seq_t *mk_vals = NULL;
static idx_t mk_cnt = 0;
static seq_t mk_bound;
static bool mk_init = false;
static bool mk_maxed = false;

static mk_trip *mk_grow_trips(mk_trip *old, idx_t oldlen, idx_t newcap) {
    mk_trip *p = mem_new(mk_trip, (size_t)newcap);
    for (idx_t i = 0; i < oldlen; i++) p[i] = old[i];
    if (old) mem_free(old);
    return p;
}

static void mk_build(seq_t B) {
    idx_t vcap = 1024, vlen = 0;
    seq_t *vals = mk_grow_vals(NULL, 0, vcap);
    idx_t scap = 1024, slen = 0;
    mk_trip *st = mk_grow_trips(NULL, 0, scap);

    vals[vlen++] = 1;
    vals[vlen++] = 2;
    st[slen].x = 1; st[slen].y = 2; st[slen].z = 5; slen++;

    while (slen > 0) {
        mk_trip t = st[--slen];
        if (vlen >= vcap) { vals = mk_grow_vals(vals, vlen, vcap * 2); vcap *= 2; }
        vals[vlen++] = t.z;
        if (slen + 2 >= scap) { st = mk_grow_trips(st, slen, scap * 2); scap *= 2; }
        seq_t c;
        if (mk_child(t.x, t.y, t.z, B, &c)) {
            st[slen].x = t.x; st[slen].y = t.z; st[slen].z = c; slen++;
        }
        if (mk_child(t.y, t.x, t.z, B, &c)) {
            st[slen].x = t.y; st[slen].y = t.z; st[slen].z = c; slen++;
        }
    }
    mem_free(st);

    mk_sort(vals, vlen);
    idx_t k = 0;
    for (idx_t i = 0; i < vlen; i++) {
        if (k == 0 || vals[i] != vals[k - 1]) { vals[k] = vals[i]; k++; }
    }
    if (mk_vals) mem_free(mk_vals);
    mk_vals = vals;
    mk_cnt = k;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (!mk_init) {
        mk_init = true;
        mk_bound = 16;
        mk_build(mk_bound);
    }
    while (mk_cnt < n) {
        if (mk_maxed) return 0;
        seq_t next = mk_bound * mk_bound;
        if (next / mk_bound != mk_bound || next <= mk_bound) {
            next = (seq_t)0 - 1;
            mk_maxed = true;
        }
        mk_bound = next;
        mk_build(mk_bound);
    }
    return mk_vals[n - 1];
}
