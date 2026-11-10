/*
 * A082283  Duplicate of A065867.
 * https://oeis.org/A082283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th prime that is a sum of a prime number of consecutive
 *            primes; computed by sieve with doubling limit
 * Range:     i8 n <= 12, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *A082283_cache = NULL;
static size_t A082283_count = 0;

static void A082283_build(uint64_t limit)
{
    uint8_t *comp = (uint8_t *)calloc((size_t)(limit + 1), 1);
    uint8_t *found = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!comp || !found) { free(comp); free(found); return; }
    comp[0] = 1; if (limit >= 1) comp[1] = 1;
    for (uint64_t i = 2; i * i <= limit; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j <= limit; j += i) comp[j] = 1;
    size_t pc = 0;
    for (uint64_t i = 2; i <= limit; i++) if (!comp[i]) pc++;
    uint64_t *pr = (uint64_t *)malloc((pc + 1) * sizeof(uint64_t));
    if (!pr) { free(comp); free(found); return; }
    size_t k = 0;
    for (uint64_t i = 2; i <= limit; i++) if (!comp[i]) pr[k++] = i;

    for (size_t s = 0; s < pc; s++) {
        uint64_t sum = 0;
        for (size_t len = 1; s + len <= pc; len++) {
            sum += pr[s + len - 1];
            if (sum > limit) break;
            if (len >= 2 && !comp[len] && !comp[sum]) found[sum] = 1;
        }
    }
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; i++) if (found[i]) cnt++;
    uint64_t *out = (uint64_t *)malloc((cnt + 1) * sizeof(uint64_t));
    if (out) {
        size_t m = 0;
        for (uint64_t i = 2; i <= limit; i++) if (found[i]) out[m++] = i;
        free(A082283_cache);
        A082283_cache = out;
        A082283_count = cnt;
    }
    free(pr); free(comp); free(found);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    static uint64_t limit = 0;
    if (limit == 0) limit = 1u << 16;
    while (A082283_count < (size_t)n) {
        size_t before = A082283_count;
        A082283_build(limit);
        if (A082283_count >= (size_t)n) break;
        if (A082283_count == before && A082283_cache == NULL && limit > ((uint64_t)1 << 34)) return (seq_t)0;
        limit <<= 1;
    }
    return (seq_t)A082283_cache[n - 1];
}
