/*
 * A004437  Numbers that are not the sum of 4 distinct squares.
 * https://oeis.org/A004437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest number of the form 4^k * m, where m (not
 *            divisible by 4) is not a sum of 4 distinct squares of
 *            nonnegative integers; the base set of such m is found by a sieve
 *            over sums of 4 distinct squares (all bases are <= 103 by the
 *            OEIS theorem)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define SIEVE_LIM 1024

typedef struct { int k; int b; } term_t;

static int bases[SIEVE_LIM];
static int nbases = -1;

static void build_bases(void) {
    static bool rep[SIEVE_LIM];
    int a, b, c, d, m;
    for (m = 0; m < SIEVE_LIM; m++) rep[m] = false;
    for (a = 0; a * a < SIEVE_LIM; a++)
        for (b = a + 1; a * a + b * b < SIEVE_LIM; b++)
            for (c = b + 1; a * a + b * b + c * c < SIEVE_LIM; c++)
                for (d = c + 1; a * a + b * b + c * c + d * d < SIEVE_LIM; d++)
                    rep[a * a + b * b + c * c + d * d] = true;
    nbases = 0;
    for (m = 1; m < SIEVE_LIM / 4; m++)
        if (m % 4 != 0 && !rep[m]) bases[nbases++] = m;
}

/* compare 4^k1*b1 with 4^k2*b2, b's < 256 */
static int cmp_term(const void *x, const void *y) {
    const term_t *p = (const term_t *)x, *q = (const term_t *)y;
    long long lhs, rhs;
    int d;
    if (p->k <= q->k) {
        d = q->k - p->k;
        if (d >= 4) return -1;
        lhs = p->b;
        rhs = (long long)q->b << (2 * d);
    } else {
        d = p->k - q->k;
        if (d >= 4) return 1;
        lhs = (long long)p->b << (2 * d);
        rhs = q->b;
    }
    return (lhs < rhs) ? -1 : (lhs > rhs) ? 1 : 0;
}

seq_t a(idx_t n) {
    idx_t K, cnt, i;
    int j, k;
    term_t *t;
    term_t res;
    seq_t v;

    if (nbases < 0) build_bases();
    K = (n - 1) / (idx_t)nbases + 5;
    cnt = K * (idx_t)nbases;
    t = (term_t *)malloc((size_t)cnt * sizeof(term_t));
    i = 0;
    for (k = 0; (idx_t)k < K; k++)
        for (j = 0; j < nbases; j++) {
            t[i].k = k;
            t[i].b = bases[j];
            i++;
        }
    qsort(t, (size_t)cnt, sizeof(term_t), cmp_term);
    res = t[n - 1];
    free(t);

    v = (seq_t)res.b;
    for (k = 0; k < res.k; k++) v *= 4;
    return v;
}
