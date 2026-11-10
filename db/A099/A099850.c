/*
 * A099850  Partial sums of A004648.
 * https://oeis.org/A099850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} A004648(k), where A004648(k) = prime(k) mod
 *            k; computed with a sieve and cached prefix sums
 * Range:     u8 n <= 29, u16 n <= 483, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8491 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

static uint64_t *A099850_pre = NULL;
static idx_t A099850_cnt = 0;

static void A099850_build(idx_t target) {
    double dn = (double)target;
    double lim_d = (target < 6) ? 15.0 : dn * (log(dn) + log(log(dn))) + 100.0;
    uint64_t limit = (uint64_t)lim_d + 10;
    unsigned char *sv = (unsigned char *)malloc((size_t)(limit + 1));
    memset(sv, 1, (size_t)(limit + 1));
    sv[0] = 0;
    sv[1] = 0;
    for (uint64_t i = 2; i * i <= limit; i++) {
        if (sv[i]) {
            for (uint64_t j = i * i; j <= limit; j += i) sv[j] = 0;
        }
    }
    uint64_t *pre = (uint64_t *)malloc((size_t)(target + 1) * sizeof(uint64_t));
    pre[0] = 0;
    idx_t k = 0;
    uint64_t sum = 0;
    for (uint64_t p = 2; p <= limit && k < target; p++) {
        if (sv[p]) {
            k++;
            sum += p % (uint64_t)k;
            pre[k] = sum;
        }
    }
    free(sv);
    free(A099850_pre);
    A099850_pre = pre;
    A099850_cnt = k;
}

seq_t a(idx_t n) {
    if (n > A099850_cnt) {
        idx_t target = n;
        if (target < 2 * A099850_cnt) target = 2 * A099850_cnt;
        if (target < 1000) target = 1000;
        A099850_build(target);
    }
    return (seq_t)A099850_pre[n];
}
