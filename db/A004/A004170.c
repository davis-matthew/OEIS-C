/*
 * A004170  Reversals of Fibonacci numbers (sorted).
 * https://oeis.org/A004170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   search, not a closed formula. Build the decimal digits of F(k)
 *            for k = 0, 1, 2, ... and reverse each one. Keep going until no
 *            later F(k) can have a reversal with at most as many digits as
 *            the current candidate. Then sort and pick the n-th value
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   a004170_flog5, a004170_step
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a004170_flog5.h"
#include "../utils/a004170_step.h"

typedef struct {
    unsigned char *s; /* most significant digit first, no leading zeros */
    size_t len;       /* 0 means the value 0 */
} a004170_rev_t;

static int a004170_cmp(const void *a, const void *b) {
    const a004170_rev_t *x = (const a004170_rev_t *)a;
    const a004170_rev_t *y = (const a004170_rev_t *)b;
    if (x->len != y->len) return x->len < y->len ? -1 : 1;
    if (x->len == 0) return 0;
    return memcmp(x->s, y->s, x->len);
}

static void a004170_add(a004170_rev_t **arr, size_t *cnt, size_t *cap,
                        const unsigned char *d, size_t len) {
    size_t tz = 0;
    a004170_rev_t r;
    while (tz < len && d[tz] == 0) tz++;
    if (*cnt == *cap) {
        *cap = *cap * 2 + 16;
        *arr = (a004170_rev_t *)realloc(*arr, *cap * sizeof(a004170_rev_t));
    }
    r.len = len - tz;
    r.s = (unsigned char *)malloc(r.len ? r.len : 1);
    if (r.len) memcpy(r.s, d + tz, r.len);
    (*arr)[(*cnt)++] = r;
}

seq_t a(idx_t n) {
    size_t capd = 64, lp = 0, lc = 1, cnt = 0, cap = 0, d, i;
    unsigned char *p = (unsigned char *)calloc(capd, 1);
    unsigned char *c = (unsigned char *)calloc(capd, 1);
    a004170_rev_t *arr = NULL;
    idx_t k = 1;
    seq_t r = 0;

    c[0] = 1;                                   /* p = F(0), c = F(1) */
    a004170_add(&arr, &cnt, &cap, p, 0);        /* reversal of F(0) = 0 */

    while ((idx_t)cnt < n + 1) {
        a004170_add(&arr, &cnt, &cap, c, lc);
        a004170_step(&p, &lp, &c, &lc, &capd);
        k++;
    }
    qsort(arr, cnt, sizeof(a004170_rev_t), a004170_cmp);
    d = arr[n].len;

    /* trailing zeros of F(k) <= v5(k) <= log5(k); continue until reversals must exceed d digits */
    while (lc <= d + 2 + (size_t)a004170_flog5(k)) {
        size_t tz = 0;
        while (tz < lc && c[tz] == 0) tz++;
        if (lc - tz <= d) a004170_add(&arr, &cnt, &cap, c, lc);
        a004170_step(&p, &lp, &c, &lc, &capd);
        k++;
    }
    qsort(arr, cnt, sizeof(a004170_rev_t), a004170_cmp);

    for (i = 0; i < arr[n].len; i++)
        r = r * 10 + (seq_t)(idx_t)arr[n].s[i];

    for (i = 0; i < cnt; i++) free(arr[i].s);
    free(arr);
    free(p);
    free(c);
    return r;
}
