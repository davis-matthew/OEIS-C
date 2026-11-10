/*
 * A089020  First value y satisfying x^2 - 3*y^2 = d^2, where d=A072330(n).
 * https://oeis.org/A089020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least y >= 1 with gcd(y, d) = 1 and 3*y^2 + d^2 a perfect
 *            square, where d = A072330(n) (primitive solution of x^2 - 3*y^2
 *            = d^2); computed by search
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 974 (2026-11-07)
 * Helpers:   gcd_u64, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt_u64.h"
#include "../utils/gcd_u64.h"

/* ---- A072330 computed natively in uint64 ---- */
static uint64_t *A072330_spf = NULL;
static uint64_t A072330_spf_limit = 0;
static uint64_t *A072330_terms = NULL;
static size_t A072330_len = 0, A072330_cap = 0;
static uint64_t A072330_last = 1;

static void A072330_ensure_spf(uint64_t limit)
{
    if (limit <= A072330_spf_limit) return;
    uint64_t nl = A072330_spf_limit ? A072330_spf_limit : 16384;
    while (nl < limit) nl <<= 1;
    uint64_t *s = (uint64_t *)calloc((size_t)nl + 1, sizeof(uint64_t));
    if (!s) exit(1);
    for (uint64_t i = 2; i * i <= nl; ++i)
        if (!s[i]) {
            s[i] = i;
            for (uint64_t j = i * i; j <= nl; j += i)
                if (!s[j]) s[j] = i;
        }
    for (uint64_t i = 2; i <= nl; ++i)
        if (!s[i]) s[i] = i;
    free(A072330_spf);
    A072330_spf = s;
    A072330_spf_limit = nl;
}

static bool A072330_allowed(uint64_t x)
{
    while (x > 1) {
        uint64_t p = A072330_spf[x], r = p % 12;
        if (r != 1 && r != 11) return false;
        do { x /= p; } while (x % p == 0);
    }
    return true;
}

static uint64_t A072330_d(idx_t n)
{
    if (!A072330_terms) {
        A072330_cap = 64;
        A072330_terms = (uint64_t *)malloc(A072330_cap * sizeof(uint64_t));
        if (!A072330_terms) exit(1);
        A072330_terms[0] = 1;
        A072330_len = 1;
        A072330_ensure_spf(16384);
    }
    while (A072330_len < n) {
        ++A072330_last;
        A072330_ensure_spf(A072330_last);
        if (A072330_allowed(A072330_last)) {
            if (A072330_len == A072330_cap) {
                A072330_cap *= 2;
                A072330_terms = (uint64_t *)realloc(A072330_terms, A072330_cap * sizeof(uint64_t));
                if (!A072330_terms) exit(1);
            }
            A072330_terms[A072330_len++] = A072330_last;
        }
    }
    return A072330_terms[n - 1];
}

seq_t a(idx_t n)
{
    uint64_t d = A072330_d(n);
    uint64_t d2 = d * d;
    for (uint64_t y = 1;; ++y) {
        if (gcd_u64(y, d) != 1) continue;
        uint64_t v = 3 * y * y + d2;
        uint64_t r = isqrt_u64(v);
        if (r * r == v) return (seq_t)y;
    }
}
