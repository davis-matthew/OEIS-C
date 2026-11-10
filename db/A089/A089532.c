/*
 * A089532  A089531 indexed by A000040.
 * https://oeis.org/A089532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that A089531(n) = A000040(k), i.e. the index of
 *            the n-th prime p with (p-3)/2 also prime; computed by a doubling
 *            sieve
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A089532_terms = NULL;
static size_t A089532_count = 0;
static uint64_t A089532_limit = 0;

static void A089532_build(uint64_t lim)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t i, j, k = 0;
    size_t cnt = 0;
    uint64_t *res;
    if (!comp) return;
    for (i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
    for (i = 5; i <= lim; i++) {
        if (!comp[i]) {
            uint64_t q = (i - 3) / 2;
            if ((i & 1) && q >= 2 && !comp[q]) cnt++;
        }
    }
    res = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    if (!res) { free(comp); return; }
    cnt = 0;
    k = 2; /* primes 2 and 3 are indices 1,2; p=5 is index 3 */
    for (i = 5; i <= lim; i++) {
        if (!comp[i]) {
            uint64_t q = (i - 3) / 2;
            k++;
            if ((i & 1) && q >= 2 && !comp[q]) res[cnt++] = k;
        }
    }
    free(comp);
    free(A089532_terms);
    A089532_terms = res;
    A089532_count = cnt;
    A089532_limit = lim;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    while ((idx_t)A089532_count < n) {
        uint64_t lim = A089532_limit < 1024 ? 1024 : A089532_limit * 2;
        size_t before = A089532_count;
        A089532_build(lim);
        if (A089532_limit != lim) return (seq_t)0;
        (void)before;
    }
    return (seq_t)A089532_terms[n - 1];
}
