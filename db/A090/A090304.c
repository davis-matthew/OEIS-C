/*
 * A090304  Indices of 0's in A090303.
 * https://oeis.org/A090304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k such that A090303(k) = 0, i.e. no k - (p1+...+pj),
 *            j>=1, is prime; found by a sieve-based search
 * Range:     u8 n <= 41, u16 n <= 214, u32 n <= 214, u64 n <= 214,
 *            u128 n <= 214, big any n
 * Validated: standard, pass, 73 terms through n = 179 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#define A090304_LIMIT 200000

static bool A090304_is_zero(uint32_t k, const unsigned char *comp, const uint32_t *ps, size_t np)
{
    if (k <= 2) return true;
    for (size_t j = 0; j < np; j++) {
        if (ps[j] > k - 2) break;
        uint32_t c = k - ps[j];
        if (c >= 2 && !comp[c]) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static unsigned char *comp = NULL;
    static uint32_t *ps = NULL;      /* prefix sums of primes */
    static size_t np = 0;
    static uint32_t *zeros = NULL;
    static size_t nz = 0, zcap = 0;
    static uint32_t next_k = 1;

    if (!comp) {
        comp = (unsigned char *)calloc(A090304_LIMIT + 1, 1);
        ps = (uint32_t *)malloc(sizeof(uint32_t) * (A090304_LIMIT + 1));
        if (!comp || !ps) abort();
        comp[0] = comp[1] = 1;
        for (uint32_t i = 2; (uint64_t)i * i <= A090304_LIMIT; i++)
            if (!comp[i])
                for (uint32_t j = i * i; j <= A090304_LIMIT; j += i) comp[j] = 1;
        uint32_t s = 0;
        for (uint32_t i = 2; i <= A090304_LIMIT; i++) {
            if (!comp[i]) {
                s += i;
                if (s > A090304_LIMIT) break;
                ps[np++] = s;
            }
        }
        zcap = 512;
        zeros = (uint32_t *)malloc(sizeof(uint32_t) * zcap);
        if (!zeros) abort();
    }

    while (nz < (size_t)n && next_k <= A090304_LIMIT) {
        uint32_t k = next_k++;
        if (A090304_is_zero(k, comp, ps, np)) {
            if (nz == zcap) {
                zcap <<= 1;
                zeros = (uint32_t *)realloc(zeros, sizeof(uint32_t) * zcap);
                if (!zeros) abort();
            }
            zeros[nz++] = k;
        }
    }
    if ((size_t)n >= 1 && (size_t)n <= nz) return (seq_t)zeros[(size_t)n - 1];
    return 0;
}
