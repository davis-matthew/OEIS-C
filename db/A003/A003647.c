/*
 * A003647  Class number of real quadratic forms with discriminant 4n+1.
 * https://oeis.org/A003647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = narrow class number h+(D) for D = n-th squarefree number
 *            > 1 that is 1 (mod 4) (computed as the number of rho-cycles of
 *            reduced indefinite forms (a,b,c), b^2-4ac = D)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   isqrt_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/isqrt_ll.h"

typedef struct { long long fa, fb, fc; } qform_t;

static int qform_cmp(const void *x, const void *y) {
    const qform_t *p = (const qform_t *)x, *q = (const qform_t *)y;
    if (p->fa != q->fa) return p->fa < q->fa ? -1 : 1;
    if (p->fb != q->fb) return p->fb < q->fb ? -1 : 1;
    return 0;
}

static int sqfree_odd(long long D) {
    long long p;
    for (p = 3; p * p <= D; p += 2)
        if (D % (p * p) == 0) return 0;
    return 1;
}

static long long nth_disc(idx_t n) {
    long long D = 1;
    idx_t cnt = 0;
    while (cnt < n) {
        D += 4;
        if (sqfree_odd(D)) cnt++;
    }
    return D;
}

static long long narrow_class_number(long long D) {
    long long s = isqrt_ll(D);
    long long cap = 64, len = 0, b, x, i, cycles = 0;
    qform_t *f = (qform_t *)malloc((size_t)cap * sizeof(qform_t));
    char *seen;
    for (b = 1; b <= s; b += 2) {
        long long m = (D - b * b) / 4;
        long long lo = (s - b + 2) / 2, hi = (s + b) / 2;
        if (lo < 1) lo = 1;
        for (x = lo; x <= hi; x++) {
            if (m % x != 0) continue;
            if (len + 2 > cap) {
                cap *= 2;
                f = (qform_t *)realloc(f, (size_t)cap * sizeof(qform_t));
            }
            f[len].fa = x;  f[len].fb = b; f[len].fc = -(m / x); len++;
            f[len].fa = -x; f[len].fb = b; f[len].fc = m / x;    len++;
        }
    }
    qsort(f, (size_t)len, sizeof(qform_t), qform_cmp);
    seen = (char *)calloc((size_t)(len > 0 ? len : 1), 1);
    for (i = 0; i < len; i++) {
        long long j;
        if (seen[i]) continue;
        cycles++;
        j = i;
        while (!seen[j]) {
            qform_t cur = f[j], key;
            long long c = cur.fc, C = 2 * (c < 0 ? -c : c);
            long long nb = s - ((s + cur.fb) % C);
            qform_t *hit;
            seen[j] = 1;
            key.fa = c;
            key.fb = nb;
            key.fc = (nb * nb - D) / (4 * c);
            hit = (qform_t *)bsearch(&key, f, (size_t)len, sizeof(qform_t), qform_cmp);
            if (!hit) break;
            j = (long long)(hit - f);
        }
    }
    free(f);
    free(seen);
    return cycles;
}

seq_t a(idx_t n) {
    long long D = nth_disc(n);
    return (seq_t)narrow_class_number(D);
}
