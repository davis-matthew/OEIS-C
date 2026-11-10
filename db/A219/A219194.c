/*
 * A219194  a(n) = max(A218075(n+1), A218075(n)) / min(A218075(n+1),
 *          A218075(n)).
 * https://oeis.org/A219194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 2^|d(n+1)-d(n)| where d(n) = prime(n+1)-prime(n); equals
 *            max/min of A218075(n+1), A218075(n)
 * Range:     u8 n <= 28, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t A219194_prime(idx_t k)
{
    static uint64_t *pr = NULL;
    static size_t sz = 0, cap = 0;
    static uint64_t cand = 3;

    if (cap == 0) {
        cap = 16;
        pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!pr) abort();
        pr[sz++] = 2;
    }
    while (sz < (size_t)k) {
        uint64_t p = cand;
        bool ok = true;
        for (size_t i = 1; i < sz; ++i) {
            uint64_t q = pr[i];
            if (q * q > p) break;
            if (p % q == 0) { ok = false; break; }
        }
        cand += 2;
        if (!ok) continue;
        if (sz == cap) {
            cap <<= 1;
            pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
    }
    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t p1 = A219194_prime(n);
    uint64_t p2 = A219194_prime(n + 1);
    uint64_t p3 = A219194_prime(n + 2);
    uint64_t d1 = p2 - p1, d2 = p3 - p2;
    unsigned diff = (unsigned)(d1 > d2 ? d1 - d2 : d2 - d1);
    seq_t r = 1;
    for (unsigned i = 0; i < diff; ++i) r = r * 2;
    return r;
}
