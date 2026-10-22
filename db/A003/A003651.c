/*
 * A003651  Class number of real quadratic field with discriminant 4n, n ==
 *          2,3 ( mod 4).
 * https://oeis.org/A003651
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(4m), the (wide) class number of Q(sqrt(m)), where m is
 *            the (n-1)-th squarefree number with m == 2,3 (mod 4). The code
 *            lists the reduced indefinite forms of discriminant D = 4m,
 *            splits them into rho-cycles (one per narrow class), and treats
 *            each cycle and the cycle of -f as one class
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct { long long a, b, c; } qform_t;

static int qf_cmp(const void *x, const void *y) {
    const qform_t *p = (const qform_t *)x, *q = (const qform_t *)y;
    if (p->a != q->a) return p->a < q->a ? -1 : 1;
    if (p->b != q->b) return p->b < q->b ? -1 : 1;
    return 0;
}

static long long qf_find(const qform_t *f, long long cnt, long long a, long long b) {
    long long lo = 0, hi = cnt - 1;
    while (lo <= hi) {
        long long mid = (lo + hi) / 2;
        if (f[mid].a == a && f[mid].b == b) return mid;
        if (f[mid].a < a || (f[mid].a == a && f[mid].b < b)) lo = mid + 1;
        else hi = mid - 1;
    }
    return -1;
}

static bool sqfree_ll(long long m) {
    long long p;
    for (p = 2; p * p <= m; p++)
        if (m % (p * p) == 0) return false;
    return true;
}

static long long isqrt_ll(long long x) {
    long long s = 0;
    while ((s + 1) * (s + 1) <= x) s++;
    return s;
}

static long long class_number(long long D) {
    long long s = isqrt_ll(D);
    long long cap = 64, cnt = 0, b, d, i, h = 0;
    qform_t *f = (qform_t *)malloc((size_t)cap * sizeof(qform_t));
    bool *seen;
    for (b = (D % 2 == 0) ? 2 : 1; b <= s; b += 2) {
        long long N = (D - b * b) / 4;
        for (d = 1; d <= N; d++) {
            long long lowok, highok, sg;
            if (N % d != 0) continue;
            /* reduced: 2d - b < sqrt(D) < 2d + b */
            lowok = (2 * d - b < 0) || ((2 * d - b) * (2 * d - b) < D);
            highok = (2 * d + b) * (2 * d + b) > D;
            if (!(lowok && highok)) continue;
            for (sg = -1; sg <= 1; sg += 2) {
                if (cnt == cap) {
                    cap *= 2;
                    f = (qform_t *)realloc(f, (size_t)cap * sizeof(qform_t));
                }
                f[cnt].a = sg * d;
                f[cnt].b = b;
                f[cnt].c = -sg * (N / d);
                cnt++;
            }
        }
    }
    qsort(f, (size_t)cnt, sizeof(qform_t), qf_cmp);
    seen = (bool *)calloc((size_t)(cnt > 0 ? cnt : 1), sizeof(bool));
    for (i = 0; i < cnt; i++) {
        long long pass;
        if (seen[i]) continue;
        h++;
        /* mark the cycle of f and the cycle of -f */
        for (pass = 0; pass < 2; pass++) {
            long long a0 = f[i].a, b0 = f[i].b, c0 = f[i].c;
            long long a1, b1, c1, j;
            if (pass == 1) { a0 = -a0; c0 = -c0; }
            a1 = a0; b1 = b0; c1 = c0;
            for (;;) {
                long long m2, r, nb, na;
                j = qf_find(f, cnt, a1, b1);
                if (j < 0 || seen[j]) break;
                seen[j] = true;
                m2 = 2 * (c1 < 0 ? -c1 : c1);
                r = (s + b1) % m2;
                if (r < 0) r += m2;
                nb = s - r;
                na = (nb * nb - D) / (4 * c1);
                a1 = c1; b1 = nb; c1 = na;
            }
        }
    }
    free(seen);
    free(f);
    return h;
}

seq_t a(idx_t n) {
    long long target = (long long)n - 1, cnt = 0, m = 1;
    while (cnt < target) {
        m++;
        if ((m % 4 == 2 || m % 4 == 3) && sqfree_ll(m)) cnt++;
    }
    return (seq_t)class_number(4 * m);
}
