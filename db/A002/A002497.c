/*
 * A002497  Numbers N in A002809 such that there is rho > 0 such that for all
 *          A > 0, A008475(A)-A008475(N) >= rho*log(A/N).
 * https://oeis.org/A002497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   no closed form; computed by enumeration. N is an l-superchampion
 *            when it minimises l(A) - rho*log(A) for some rho > 0. Each prime
 *            p contributes its own exponent, so the terms are running
 *            products over the "events" sorted by threshold: p/log p (factor
 *            p), (p^k - p^(k-1))/log p (factor p), and for...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

typedef struct { long double key; unsigned f; } a2497_ev;

static a2497_ev *a2497_E = NULL;
static size_t a2497_En = 0;

static int a2497_cmp(const void *x, const void *y)
{
    const a2497_ev *u = (const a2497_ev *)x, *v = (const a2497_ev *)y;
    if (u->key < v->key) return -1;
    if (u->key > v->key) return 1;
    return 0;
}

static void a2497_build(idx_t n)
{
    size_t L = 4096;
    for (;;) {
        unsigned char *comp = (unsigned char *)calloc(L + 1, 1);
        size_t i, j, cap = 1024, cnt = 0;
        a2497_ev *ev = (a2497_ev *)malloc(cap * sizeof(a2497_ev));
        long double Kmax = (long double)L / logl((long double)L);
        for (i = 2; i * i <= L; i++)
            if (!comp[i])
                for (j = i * i; j <= L; j += i) comp[j] = 1;
        for (i = 2; i <= L; i++) {
            long double lp, q, key;
            unsigned k;
            if (comp[i]) continue;
            lp = logl((long double)i);
            if (i == 2) {
                key = 2.0L / lp;
                if (key > Kmax) continue;
                if (cnt == cap) { cap *= 2; ev = (a2497_ev *)realloc(ev, cap * sizeof(a2497_ev)); }
                ev[cnt].key = key; ev[cnt].f = 4; cnt++;
                q = 4.0L; /* 2^(k-1) for k = 3 */
                for (;;) {
                    key = q / lp;
                    if (key > Kmax) break;
                    if (cnt == cap) { cap *= 2; ev = (a2497_ev *)realloc(ev, cap * sizeof(a2497_ev)); }
                    ev[cnt].key = key; ev[cnt].f = 2; cnt++;
                    q *= 2.0L;
                }
                continue;
            }
            key = (long double)i / lp;
            if (key > Kmax) continue;
            if (cnt == cap) { cap *= 2; ev = (a2497_ev *)realloc(ev, cap * sizeof(a2497_ev)); }
            ev[cnt].key = key; ev[cnt].f = (unsigned)i; cnt++;
            q = (long double)i; /* p^(k-1) */
            for (k = 2;; k++) {
                key = q * (long double)(i - 1) / lp;
                if (key > Kmax) break;
                if (cnt == cap) { cap *= 2; ev = (a2497_ev *)realloc(ev, cap * sizeof(a2497_ev)); }
                ev[cnt].key = key;
                ev[cnt].f = (i == 5 && k == 2) ? 1u : (unsigned)i;
                cnt++;
                q *= (long double)i;
            }
        }
        free(comp);
        if (cnt >= n) {
            qsort(ev, cnt, sizeof(a2497_ev), a2497_cmp);
            free(a2497_E);
            a2497_E = ev;
            a2497_En = cnt;
            return;
        }
        free(ev);
        L *= 2;
    }
}

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t i;
    if (n == 0) return 0;
    if (a2497_En < n) a2497_build(n);
    for (i = 0; i < n; i++) r *= (seq_t)a2497_E[i].f;
    return r;
}
