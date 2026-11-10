/*
 * A239324  Partial sums of A090431.
 * https://oeis.org/A239324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} (digitsum(k) - digitsum(prime(k))), a(0) = 0
 * Range:     i8 n <= 43, i16 n <= 6843, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 9209 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static unsigned A239324_ds(uint64_t x)
{
    unsigned s = 0;
    while (x) { s += (unsigned)(x % 10); x /= 10; }
    return s;
}

static uint32_t *A239324_pr = NULL;
static size_t A239324_sz = 0, A239324_cap = 0;

static void A239324_ensure(size_t k)
{
    uint32_t cand;
    if (A239324_cap == 0) {
        A239324_cap = 64;
        A239324_pr = (uint32_t *)malloc(A239324_cap * sizeof(uint32_t));
        if (!A239324_pr) abort();
    }
    if (A239324_sz == 0) A239324_pr[A239324_sz++] = 2;
    cand = A239324_pr[A239324_sz - 1] + (A239324_sz == 1 ? 1 : 2);
    while (A239324_sz < k) {
        bool ok = true;
        for (size_t i = 1; i < A239324_sz; ++i) {
            uint64_t q = A239324_pr[i];
            if (q * q > cand) break;
            if (cand % q == 0) { ok = false; break; }
        }
        if (ok) {
            if (A239324_sz == A239324_cap) {
                A239324_cap <<= 1;
                A239324_pr = (uint32_t *)realloc(A239324_pr, A239324_cap * sizeof(uint32_t));
                if (!A239324_pr) abort();
            }
            A239324_pr[A239324_sz++] = cand;
        }
        cand += 2;
    }
}

seq_t a(idx_t n)
{
    seq_t s = 0;
    if (n == 0) return s;
    A239324_ensure((size_t)n);
    for (idx_t k = 1; k <= n; ++k) {
        long long d = (long long)A239324_ds(k) - (long long)A239324_ds(A239324_pr[k - 1]);
        s += (seq_t)d;
    }
    return s;
}
