/*
 * A003648  Class number of quadratic forms with discriminant 4n, n == 2,3^(
 *          mod 4).
 * https://oeis.org/A003648
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = narrow (proper SL2(Z)) class number h+(4m) of primitive
 *            binary quadratic forms of discriminant 4m, where m is the
 *            (n-1)-th squarefree number with m == 2,3 (mod 4) (m = 2, 3, 6,
 *            7, 10, 11, 14, 15, 19, ...); computed by enumerating the reduced
 *            indefinite forms and counting their cycles under...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   a3648_gcd, a3648_sqfree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3648_gcd.h"
#include "../utils/a3648_sqfree.h"

static long long *a3648_ms = 0;
static size_t a3648_mcount = 0, a3648_mcap = 0;
static long long a3648_last = 1;

static long long a3648_nth_m(size_t k) {
    while (a3648_mcount <= k) {
        long long m = a3648_last + 1;
        while (!((m % 4 == 2 || m % 4 == 3) && a3648_sqfree(m))) m++;
        a3648_last = m;
        if (a3648_mcount == a3648_mcap) {
            size_t nc = a3648_mcap ? 2 * a3648_mcap : 1024;
            a3648_ms = (long long *)realloc(a3648_ms, nc * sizeof(long long));
            a3648_mcap = nc;
        }
        a3648_ms[a3648_mcount++] = m;
    }
    return a3648_ms[k];
}

typedef struct { long long a, b, c; } a3648_form;

static int a3648_cmp(const void *p, const void *q) {
    const a3648_form *x = (const a3648_form *)p, *y = (const a3648_form *)q;
    if (x->a != y->a) return x->a < y->a ? -1 : 1;
    if (x->b != y->b) return x->b < y->b ? -1 : 1;
    return 0;
}

static long long a3648_find(a3648_form *f, long long cnt, long long a, long long b) {
    long long lo = 0, hi = cnt - 1;
    while (lo <= hi) {
        long long mid = (lo + hi) / 2;
        if (f[mid].a == a && f[mid].b == b) return mid;
        if (f[mid].a < a || (f[mid].a == a && f[mid].b < b)) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

static long long a3648_hplus(long long m) {
    long long D = 4 * m, s = 0, b, a, cnt = 0, cap = 256, h = 0, i;
    a3648_form *f;
    char *seen;
    while ((s + 1) * (s + 1) <= D) s++;
    f = (a3648_form *)malloc((size_t)cap * sizeof(a3648_form));
    for (b = 2; b <= s; b += 2) {
        long long N = (D - b * b) / 4;
        for (a = 1; a <= s; a++) {
            long long c, t;
            if (N % a) continue;
            if ((2 * a + b) * (2 * a + b) <= D) continue;
            t = 2 * a - b;
            if (t > 0 && t * t >= D) continue;
            c = N / a;
            if (a3648_gcd(a3648_gcd(a, b), c) != 1) continue;
            if (cnt + 2 > cap) {
                cap *= 2;
                f = (a3648_form *)realloc(f, (size_t)cap * sizeof(a3648_form));
            }
            f[cnt].a = a;  f[cnt].b = b; f[cnt].c = -c; cnt++;
            f[cnt].a = -a; f[cnt].b = b; f[cnt].c = c;  cnt++;
        }
    }
    qsort(f, (size_t)cnt, sizeof(a3648_form), a3648_cmp);
    seen = (char *)calloc((size_t)(cnt ? cnt : 1), 1);
    for (i = 0; i < cnt; i++) {
        long long j = i;
        if (seen[i]) continue;
        h++;
        while (j >= 0 && !seen[j]) {
            long long c = f[j].c, ac = c < 0 ? -c : c, md = 2 * ac;
            long long t = (s + f[j].b) % md, nb, nc;
            seen[j] = 1;
            nb = s - t;
            nc = (nb * nb - D) / (4 * c);
            (void)nc;
            j = a3648_find(f, cnt, c, nb);
        }
    }
    free(seen);
    free(f);
    return h;
}

seq_t a(idx_t n) {
    long long m = a3648_nth_m((size_t)(n - 2));
    return (seq_t)a3648_hplus(m);
}
