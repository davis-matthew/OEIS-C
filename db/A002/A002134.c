/*
 * A002134  Generalized divisor function. Number of partitions of n with
 *          exactly three part sizes.
 * https://oeis.org/A002134
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   with d = A000005, sigma = A000203, dd(m) = Sum_{x=1..m-1}
 *            d(x)*d(m-x), T(n) = Sum_{z=1..n-2} dd(n-z)*d(z) (ordered triples
 *            a*i+b*j+c*k = n), P(n) = Sum_{m=2..n-1} (sigma(m)-d(m))*d(n-m)
 *            (ordered triples with i = j), Q(n) = Sum_{i|n} binomial(n/i-1,
 *            2) (ordered triples with i = j = k), a(n) = (T(n...
 * Range:     u8 n <= 19, u16 n <= 113, u32 n <= 8699, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *a2134_d = NULL;
static uint64_t *a2134_sg = NULL;
static uint64_t *a2134_dd = NULL;
static idx_t a2134_cap = 0;

static void a2134_ensure(idx_t need) {
    idx_t oldcap, newcap, i, j, m;
    uint64_t *nd, *ns, *ndd;
    if (need <= a2134_cap) return;
    oldcap = a2134_cap;
    newcap = need;
    nd = (uint64_t *)calloc((size_t)newcap, sizeof(uint64_t));
    ns = (uint64_t *)calloc((size_t)newcap, sizeof(uint64_t));
    ndd = (uint64_t *)calloc((size_t)newcap, sizeof(uint64_t));
    for (i = 1; i < newcap; i++)
        for (j = i; j < newcap; j += i) {
            nd[j]++;
            ns[j] += i;
        }
    if (a2134_dd != NULL && oldcap > 0)
        memcpy(ndd, a2134_dd, (size_t)oldcap * sizeof(uint64_t));
    for (m = (oldcap > 2 ? oldcap : 2); m < newcap; m++) {
        uint64_t s = 0;
        for (i = 1; i < m; i++) s += nd[i] * nd[m - i];
        ndd[m] = s;
    }
    free(a2134_d);
    free(a2134_sg);
    free(a2134_dd);
    a2134_d = nd;
    a2134_sg = ns;
    a2134_dd = ndd;
    a2134_cap = newcap;
}

seq_t a(idx_t n) {
    uint64_t T = 0, P = 0, Q = 0, num;
    idx_t z, m, i;
    seq_t r = 0;
    if (n < 6) return r;
    a2134_ensure(n + 1);
    for (z = 1; z + 2 <= n; z++) T += a2134_dd[n - z] * a2134_d[z];
    for (m = 2; m < n; m++) P += (a2134_sg[m] - a2134_d[m]) * a2134_d[n - m];
    for (i = 1; i <= n; i++) {
        if (n % i == 0) {
            uint64_t q = n / i;
            if (q >= 3) Q += (q - 1) * (q - 2) / 2;
        }
    }
    num = (T + 2 * Q - 3 * P) / 6;
    r += (idx_t)num;
    return r;
}
