/*
 * A200247  Partial sums of A200246.
 * https://oeis.org/A200247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A200246(k), where A200246(k) = parity of the
 *            binary weight of prime(k); computed with a sieve and cached
 *            prefix sums
 * Range:     u8 n <= 435, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 85 terms through n = 978 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A200247_ps = NULL;
static size_t A200247_cnt = 0;
static uint64_t A200247_limit = 0;

static void A200247_build(uint64_t limit)
{
    unsigned char *comp = (unsigned char *)calloc((size_t)limit + 1, 1);
    if (!comp) abort();
    size_t maxp = (size_t)limit / 2 + 2;
    uint32_t *ps = (uint32_t *)malloc(maxp * sizeof(uint32_t));
    if (!ps) abort();
    size_t c = 0;
    uint32_t sum = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (comp[i]) continue;
        if (i * i <= limit)
            for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
        uint64_t p = i;
        unsigned par = 0;
        while (p) { par ^= 1; p &= p - 1; }
        sum += par;
        ps[c++] = sum;
    }
    free(comp);
    free(A200247_ps);
    A200247_ps = ps;
    A200247_cnt = c;
    A200247_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t lim = A200247_limit ? A200247_limit : 1024;
    while (A200247_limit == 0 || A200247_cnt < (size_t)n) {
        if (A200247_limit != 0) lim = A200247_limit * 2;
        A200247_build(lim);
    }
    return (seq_t)A200247_ps[(size_t)n - 1];
}
