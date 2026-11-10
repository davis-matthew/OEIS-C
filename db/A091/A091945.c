/*
 * A091945  Numbers n such that there is no k, 0 <= k < n, satisfying
 *          A002487(k) = A002487(n).
 * https://oeis.org/A091945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with A002487(k) = m for the n-th distinct value m
 *            in order of first occurrence; computed by sieving Stern's
 *            diatomic series (s(0)=0, s(1)=1, s(2k)=s(k),
 *            s(2k+1)=s(k)+s(k+1)) and listing indices k where s(k) has not
 *            appeared at any smaller index
 * Range:     u8 n <= 32, u16 n <= 1176, u32 n <= 7297, u64 n <= 7297,
 *            u128 n <= 7297, big any n
 * Validated: standard, pass, 74 terms through n = 6611 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *A091945_terms = 0;
static size_t A091945_count = 0;

static void A091945_build(size_t cap) {
    uint32_t *s = (uint32_t *)malloc(cap * sizeof(uint32_t));
    unsigned char *seen = (unsigned char *)calloc(cap + 2, 1);
    uint64_t *t = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t cnt = 0, k;
    s[0] = 0;
    if (cap > 1) s[1] = 1;
    for (k = 2; k < cap; k++) {
        if (k & 1) s[k] = s[k / 2] + s[k / 2 + 1];
        else s[k] = s[k / 2];
    }
    for (k = 0; k < cap; k++) {
        uint32_t v = s[k];
        if (!seen[v]) {
            seen[v] = 1;
            t[cnt++] = (uint64_t)k;
        }
    }
    free(s);
    free(seen);
    free(A091945_terms);
    A091945_terms = t;
    A091945_count = cnt;
}

seq_t a(idx_t n) {
    static size_t cap = 0;
    if (cap == 0) cap = 1 << 12;
    while (A091945_terms == 0 || (size_t)n > A091945_count) {
        A091945_build(cap);
        if ((size_t)n <= A091945_count) break;
        cap *= 2;
    }
    return (seq_t)A091945_terms[(size_t)n - 1];
}
