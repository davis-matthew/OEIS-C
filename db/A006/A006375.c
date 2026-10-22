/*
 * A006375  Number of equivalence classes of cycles (or periods) of reduced
 *          indefinite binary quadratic forms of determinant -n (see
 *          comments).
 * https://oeis.org/A006375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = number of classes of all forms [a,b,c]
 *            (a*x^2+2*b*x*y+c*y^2, b^2-a*c = n) under GL2(Z) equivalence plus
 *            [a,b,c]~[-a,b,-c]. Non-square n: list Gauss-reduced forms (0 < b
 *            < sqrt(n), sqrt(n)-b < |a| < sqrt(n)+b), join each to its period
 *            successor and to [-a,b,-c], [c,b,a], [-c,b,-a] with unio...
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   a000037_isqrt, gcd_ll, inv_mod, square_case, uf_join, uf_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/a000037_isqrt.h"
#include "../utils/gcd_ll.h"
#include "../utils/inv_mod.h"
#include "../utils/square_case.h"
#include "../utils/uf_join.h"
#include "../utils/uf_root.h"

typedef struct { long long fa, fb, fc; } QForm;

static int qf_cmp(const void *x, const void *y) {
    const QForm *p = (const QForm *)x, *q = (const QForm *)y;
    if (p->fa != q->fa) return p->fa < q->fa ? -1 : 1;
    if (p->fb != q->fb) return p->fb < q->fb ? -1 : 1;
    return 0;
}

static long long qf_find(const QForm *arr, long long cnt, long long fa, long long fb) {
    long long lo = 0, hi = cnt - 1;
    while (lo <= hi) {
        long long mid = (lo + hi) / 2;
        const QForm *m = &arr[mid];
        if (m->fa == fa && m->fb == fb) return mid;
        if (m->fa < fa || (m->fa == fa && m->fb < fb)) lo = mid + 1; else hi = mid - 1;
    }
    return -1;
}

seq_t a(idx_t n) {
    long long N = (long long)n;
    long long s = (long long)a000037_isqrt((uint64_t)n);
    if (s * s == N) return (seq_t)square_case(s);

    long long cap = 2 * s * (s + 1) + 4;
    QForm *fs = (QForm *)malloc((size_t)cap * sizeof(QForm));
    long long cnt = 0;
    for (long long b = 1; b <= s; b++) {
        long long r = N - b * b;
        for (long long x = s - b + 1; x <= s + b; x++) {
            if (x <= 0 || r % x) continue;
            long long y = r / x;
            fs[cnt].fa = x;  fs[cnt].fb = b; fs[cnt].fc = -y; cnt++;
            fs[cnt].fa = -x; fs[cnt].fb = b; fs[cnt].fc = y;  cnt++;
        }
    }
    qsort(fs, (size_t)cnt, sizeof(QForm), qf_cmp);

    long long *par = (long long *)malloc((size_t)(cnt + 1) * sizeof(long long));
    for (long long i = 0; i < cnt; i++) par[i] = i;

    for (long long i = 0; i < cnt; i++) {
        long long fa = fs[i].fa, fb = fs[i].fb, fc = fs[i].fc;
        long long ac = fc < 0 ? -fc : fc;
        long long nb = s - ((s + fb) % ac);
        uf_join(par, i, qf_find(fs, cnt, fc, nb));
        uf_join(par, i, qf_find(fs, cnt, -fa, fb));
        uf_join(par, i, qf_find(fs, cnt, fc, fb));
        uf_join(par, i, qf_find(fs, cnt, -fc, fb));
    }

    long long comps = 0;
    for (long long i = 0; i < cnt; i++) if (uf_root(par, i) == i) comps++;
    free(par);
    free(fs);
    return (seq_t)comps;
}
