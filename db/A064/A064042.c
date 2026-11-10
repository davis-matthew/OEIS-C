/*
 * A064042  At step n in computing A064413, sequence gives smallest multiple
 *          of 5 not yet seen, divided by 5.
 * https://oeis.org/A064042
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = m/5 where m is the smallest multiple of 5 not among the
 *            first n terms of the EKG sequence A064413 (A064413(1)=1,
 *            A064413(2)=2, then smallest unused number sharing a factor with
 *            the previous term); computed by simulation
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *A064042_res = NULL;
static size_t A064042_cap = 0;

static void A064042_build(size_t cap)
{
    size_t L = 4 * cap + 100;
    uint8_t *used = (uint8_t *)calloc(L + 2, 1);
    uint32_t *ptr = (uint32_t *)calloc(L + 2, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(cap + 2, sizeof(uint32_t));
    if (!used || !ptr || !res) exit(1);
    uint32_t m = 5;
    uint32_t prev = 0;
    for (size_t k = 1; k <= cap; ++k) {
        uint32_t cur;
        if (k == 1) cur = 1;
        else if (k == 2) cur = 2;
        else {
            uint32_t x = prev;
            uint32_t best = UINT32_MAX;
            for (uint32_t p = 2; (uint64_t)p * p <= x || x > 1; ) {
                if ((uint64_t)p * p > x) p = x;
                if (x % p == 0) {
                    while (x % p == 0) x /= p;
                    uint32_t c = ptr[p] ? ptr[p] : p;
                    while (c <= L && used[c]) c += p;
                    ptr[p] = c;
                    if (c < best) best = c;
                }
                if (x <= 1) break;
                p++;
            }
            cur = best;
        }
        used[cur] = 1;
        prev = cur;
        while (m <= L && used[m]) m += 5;
        res[k] = m / 5;
    }
    free(used);
    free(ptr);
    if (A064042_res) free(A064042_res);
    A064042_res = res;
    A064042_cap = cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if ((size_t)n > A064042_cap) {
        size_t cap = A064042_cap * 2;
        if (cap < 1024) cap = 1024;
        if (cap < (size_t)n) cap = (size_t)n;
        A064042_build(cap);
    }
    return (seq_t)A064042_res[(size_t)n];
}
