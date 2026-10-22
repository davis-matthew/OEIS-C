/*
 * A000736  Boustrophedon transform of Catalan numbers 1, 1, 1, 2, 5, 14, ...
 * https://oeis.org/A000736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of b = 1, 1, 1, 2, 5, 14, ... (b(0) = 1,
 *            b(k) = C(k-1) for k >= 1, C = Catalan): T(k,0) = b(k), T(k,j) =
 *            T(k,j-1) + T(k-1,k-j) for 1 <= j <= k, a(n) = T(n,n); Catalan
 *            numbers by C(0) = 1, C(m+1) = C(m)*2*(2m+1)/(m+2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
typedef std::vector<seq_t> a736_buf;
static void a736_resize(a736_buf *b, size_t n) {
    if (b->size() < n) b->resize(n);
}
static seq_t *a736_ptr(a736_buf *b) { return b->data(); }
#else
typedef struct { seq_t *p; size_t n; } a736_buf;
static void a736_resize(a736_buf *b, size_t n) {
    if (n > b->n) {
        size_t i;
        b->p = (seq_t *)realloc(b->p, n * sizeof(seq_t));
        for (i = b->n; i < n; i++) b->p[i] = 0;
        b->n = n;
    }
}
static seq_t *a736_ptr(a736_buf *b) { return b->p; }
#endif

static a736_buf a736_rows[2];
static a736_buf a736_vals;
static int a736_cur = 0;       /* index of buffer holding the last computed row */
static idx_t a736_cnt = 0;     /* rows 0 .. a736_cnt-1 computed */
static seq_t a736_cat = 1;     /* Catalan C(a736_catm) */
static idx_t a736_catm = 0;

static uint64_t a736_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static void a736_cat_step(void) {
    uint64_t m = a736_catm;
    uint64_t num = 2 * (2 * m + 1), den = m + 2;
    uint64_t g = a736_gcd(num, den);
    a736_cat = a736_cat / (seq_t)(den / g);
    a736_cat = a736_cat * (seq_t)(num / g);
    a736_catm++;
}

seq_t a(idx_t n) {
    if (n < a736_cnt) return a736_ptr(&a736_vals)[n];

    a736_resize(&a736_rows[0], (size_t)n + 1);
    a736_resize(&a736_rows[1], (size_t)n + 1);
    a736_resize(&a736_vals, (size_t)n + 1);
    seq_t *vals = a736_ptr(&a736_vals);

    while (a736_cnt <= n) {
        idx_t k = a736_cnt;
        seq_t *nw = a736_ptr(&a736_rows[1 - a736_cur]);
        if (k == 0) {
            nw[0] = 1;
        } else {
            seq_t *pv = a736_ptr(&a736_rows[a736_cur]);
            idx_t j;
            while (a736_catm < k - 1) a736_cat_step();
            nw[0] = a736_cat;
            for (j = 1; j <= k; j++)
                nw[j] = nw[j - 1] + pv[k - j];
        }
        vals[k] = nw[k];
        a736_cur = 1 - a736_cur;
        a736_cnt++;
    }
    return vals[n];
}
