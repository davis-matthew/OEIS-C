/*
 * A076599  Numbers n such that A068340(n) = +-10.
 * https://oeis.org/A076599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th k >= 1 with |A068340(k)| = 10, where A068340(k) =
 *            Sum_{j=1..k} mu(j)*j; found by a sieve of mu with running sum
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static idx_t *A076599_terms = NULL;
static idx_t A076599_count = 0;
static idx_t A076599_limit = 0;

static void A076599_extend(idx_t L)
{
    int8_t *mu = (int8_t *)malloc((size_t)L + 1);
    uint8_t *comp = (uint8_t *)calloc((size_t)L + 1, 1);
    if (!mu || !comp) exit(1);
    for (idx_t i = 0; i <= L; ++i) mu[i] = 1;
    for (idx_t i = 2; i <= L; ++i) {
        if (!comp[i]) {
            for (idx_t j = i; j <= L; j += i) {
                if (j != i) comp[j] = 1;
                mu[j] = (int8_t)-mu[j];
            }
            if (i <= L / i) {
                idx_t sq = i * i;
                for (idx_t j = sq; j <= L; j += sq) mu[j] = 0;
            }
        }
    }
    idx_t cap = 1024;
    idx_t *t = (idx_t *)malloc(sizeof(idx_t) * (size_t)cap);
    if (!t) exit(1);
    idx_t cnt = 0;
    long long s = 0;
    for (idx_t k = 1; k <= L; ++k) {
        s += (long long)mu[k] * (long long)k;
        if (s == 10 || s == -10) {
            if (cnt == cap) {
                cap *= 2;
                t = (idx_t *)realloc(t, sizeof(idx_t) * (size_t)cap);
                if (!t) exit(1);
            }
            t[cnt++] = k;
        }
    }
    free(mu);
    free(comp);
    free(A076599_terms);
    A076599_terms = t;
    A076599_count = cnt;
    A076599_limit = L;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A076599_limit == 0) A076599_extend(4096);
    while (A076599_count < n && A076599_limit < ((idx_t)1 << 32))
        A076599_extend(A076599_limit * 2);
    if (n > A076599_count) return (seq_t)0;
    return (seq_t)A076599_terms[n - 1];
}
