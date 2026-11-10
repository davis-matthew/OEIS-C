/*
 * A206035  a(n) = floor(10*(A064413(n) - n)/n).
 * https://oeis.org/A206035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, less
 * Formula:   a(n) = floor(10*(A064413(n) - n)/n), where A064413 is the EKG
 *            sequence (computed by direct simulation with per-prime pointers
 *            to the smallest unused multiple)
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint32_t *A206035_terms = NULL;
static size_t A206035_len = 0;

static void A206035_build(size_t n)
{
    size_t target = n * 2 + 16;
    size_t cap = target * 4 + 100;
    uint32_t *terms = (uint32_t *)malloc((target + 2) * sizeof(uint32_t));
    unsigned char *used = (unsigned char *)calloc(cap + 2, 1);
    uint32_t *ptr = (uint32_t *)calloc(cap + 2, sizeof(uint32_t));
    if (!terms || !used || !ptr) exit(1);
    terms[1] = 1; terms[2] = 2;
    used[1] = used[2] = 1;
    for (size_t i = 3; i <= target; ++i) {
        uint32_t x = terms[i - 1];
        uint32_t best = 0xFFFFFFFFu;
        for (uint32_t p = 2; (uint64_t)p * p <= x || x > 1; ++p) {
            if (x % p) {
                if ((uint64_t)p * p > x) { p = x - 1; }
                continue;
            }
            while (x % p == 0) x /= p;
            uint32_t c = ptr[p] ? ptr[p] : p;
            while (used[c]) c += p;
            ptr[p] = c;
            if (c < best) best = c;
        }
        used[best] = 1;
        terms[i] = best;
    }
    free(used);
    free(ptr);
    free(A206035_terms);
    A206035_terms = terms;
    A206035_len = target;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    if ((size_t)n > A206035_len) A206035_build((size_t)n);
    long long e = (long long)A206035_terms[n];
    long long nn = (long long)n;
    long long num = 10 * (e - nn);
    long long q = num / nn;
    if (num % nn != 0 && num < 0) q--;
    return (seq_t)q;
}
