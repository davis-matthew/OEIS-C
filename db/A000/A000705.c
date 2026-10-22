/*
 * A000705  n-th superior highly composite number A002201(n) is product of
 *          first n terms of this sequence.
 * https://oeis.org/A000705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n-th prime in the merge of all (p, k), p prime, k >= 1,
 *            sorted by decreasing critical exponent log(1+1/k)/log(p)
 *            (Ramanujan's superior highly composite construction); computed
 *            by a priority-queue merge over a prime sieve
 * Range:     u8 n <= 79, u16 n <= 6737, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

typedef struct { long double key; unsigned long long p; unsigned long long k; } a705_node;

static unsigned long long *a705_vals = NULL;
static idx_t a705_cnt = 0;

static void a705_push(a705_node *h, size_t *sz, a705_node x) {
    size_t i = (*sz)++;
    h[i] = x;
    while (i > 0) {
        size_t par = (i - 1) / 2;
        if (h[par].key >= h[i].key) break;
        a705_node t = h[par]; h[par] = h[i]; h[i] = t;
        i = par;
    }
}

static a705_node a705_pop(a705_node *h, size_t *sz) {
    a705_node top = h[0];
    h[0] = h[--(*sz)];
    size_t i = 0;
    for (;;) {
        size_t l = 2 * i + 1, r = l + 1, m = i;
        if (l < *sz && h[l].key > h[m].key) m = l;
        if (r < *sz && h[r].key > h[m].key) m = r;
        if (m == i) break;
        a705_node t = h[m]; h[m] = h[i]; h[i] = t;
        i = m;
    }
    return top;
}

static void a705_compute(idx_t N) {
    /* sieve enough primes: at most N fresh primes are needed */
    unsigned long long bound;
    if (N < 6) bound = 20;
    else {
        double dn = (double)N;
        bound = (unsigned long long)(dn * (log(dn) + log(log(dn)))) + 20;
    }
    bool *comp = (bool *)calloc(bound + 1, sizeof(bool));
    unsigned long long *primes = (unsigned long long *)malloc((N + 1) * sizeof(unsigned long long));
    idx_t np = 0;
    for (unsigned long long i = 2; i <= bound && np < N + 1; i++) {
        if (!comp[i]) {
            primes[np++] = i;
            for (unsigned long long j = i * i; j <= bound; j += i) comp[j] = true;
        }
    }
    free(comp);

    a705_node *heap = (a705_node *)malloc((N + 2) * sizeof(a705_node));
    size_t hs = 0;
    unsigned long long *vals = (unsigned long long *)malloc((N + 1) * sizeof(unsigned long long));
    idx_t fresh = 0;
    long double ln2 = logl(2.0L);
    for (idx_t t = 0; t < N; t++) {
        long double fk = ln2 / logl((long double)primes[fresh]);
        if (hs == 0 || fk > heap[0].key) {
            unsigned long long p = primes[fresh++];
            vals[t] = p;
            a705_node x;
            x.p = p; x.k = 2;
            x.key = logl(1.5L) / logl((long double)p);
            a705_push(heap, &hs, x);
        } else {
            a705_node x = a705_pop(heap, &hs);
            vals[t] = x.p;
            x.k++;
            x.key = logl((long double)(x.k + 1) / (long double)x.k) / logl((long double)x.p);
            a705_push(heap, &hs, x);
        }
    }
    free(heap);
    free(primes);
    free(a705_vals);
    a705_vals = vals;
    a705_cnt = N;
}

seq_t a(idx_t n) {
    if (n > a705_cnt) {
        idx_t N = a705_cnt * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        a705_compute(N);
    }
    return (seq_t)a705_vals[n - 1];
}
