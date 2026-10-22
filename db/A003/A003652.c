/*
 * A003652  Class number of real quadratic field with discriminant A003658(n),
 *          n >= 2.
 * https://oeis.org/A003652
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(D), D = A003658(n): the narrow class number h+ is the
 *            number of cycles of reduced indefinite forms of discriminant D;
 *            h = h+ if the principal cycle contains a form with a = -1
 *            (N(eps) = -1), else h+/2
 * Range:     u8 n <= 3001, u16 n <= 3001, u32 n <= 3001, u64 n <= 3001,
 *            u128 n <= 3001, big any n
 * Helpers:   is_fund_disc, isqrt_ll, nth_fund_disc, squarefree_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_fund_disc.h"
#include "../utils/isqrt_ll.h"
#include "../utils/nth_fund_disc.h"
#include "../utils/squarefree_ll.h"

static long long class_number(long long D) {
    long long s = isqrt_ll(D);
    long long cap = 64, N = 0, b, A, i;
    long long *fa = (long long *)malloc(sizeof(long long) * cap);
    long long *fb = (long long *)malloc(sizeof(long long) * cap);
    long long *fc = (long long *)malloc(sizeof(long long) * cap);
    long long *cyc;
    long long cycles = 0, pcycle = -1, b0, pidx = -1, h;
    bool neg = false;

    for (b = 1; b <= s; b++) {
        long long prod;
        if ((b - D) % 2 != 0) continue;
        prod = (D - b * b) / 4;
        for (A = 1; 2 * A <= s + b; A++) {
            int sg;
            if ((2 * A + b) * (2 * A + b) <= D) continue;
            if (2 * A - b > 0 && (2 * A - b) * (2 * A - b) >= D) continue;
            if (prod % A != 0) continue;
            for (sg = 0; sg < 2; sg++) {
                if (N + 1 > cap) {
                    cap *= 2;
                    fa = (long long *)realloc(fa, sizeof(long long) * cap);
                    fb = (long long *)realloc(fb, sizeof(long long) * cap);
                    fc = (long long *)realloc(fc, sizeof(long long) * cap);
                }
                fa[N] = sg ? -A : A;
                fb[N] = b;
                fc[N] = sg ? prod / A : -(prod / A);
                N++;
            }
        }
    }

    cyc = (long long *)malloc(sizeof(long long) * (N > 0 ? N : 1));
    for (i = 0; i < N; i++) cyc[i] = -1;

    for (i = 0; i < N; i++) {
        long long cur;
        if (cyc[i] >= 0) continue;
        cur = i;
        while (cyc[cur] < 0) {
            long long c = fc[cur], m = 2 * (c < 0 ? -c : c);
            long long nb = s - ((s + fb[cur]) % m);
            long long na = (nb * nb - D) / (4 * c);
            long long j;
            cyc[cur] = cycles;
            for (j = 0; j < N; j++)
                if (fa[j] == c && fb[j] == nb && fc[j] == na) break;
            if (j == N) break; /* should not happen */
            cur = j;
        }
        cycles++;
    }

    b0 = ((s - D) % 2 == 0) ? s : s - 1;
    for (i = 0; i < N; i++)
        if (fa[i] == 1 && fb[i] == b0) { pidx = i; break; }
    if (pidx >= 0) {
        pcycle = cyc[pidx];
        for (i = 0; i < N; i++)
            if (cyc[i] == pcycle && fa[i] == -1) { neg = true; break; }
    }

    h = neg ? cycles : cycles / 2;
    free(fa); free(fb); free(fc); free(cyc);
    return h;
}

seq_t a(idx_t n) {
    long long D = nth_fund_disc(n);
    return (seq_t)class_number(D);
}
