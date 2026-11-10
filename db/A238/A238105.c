/*
 * A238105  A238104(n)/3.
 * https://oeis.org/A238105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A238104(n)/3, where A238104(n) is the sum of digits in
 *            the periodic part of the decimal expansion of 1/prime(n) (0 for
 *            primes 2 and 5). Computed by long division until the remainder
 *            returns to 1
 * Range:     u8 n <= 40, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t A238105_get_prime(idx_t k)
{
    static uint32_t *pr = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static uint32_t cand = 3;

    if (cap == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
    }

    while (sz < (size_t)k) {
        if (sz == 0) {
            pr[sz++] = 2;
            continue;
        }
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t q = pr[i];
            if ((uint64_t)q * q > (uint64_t)p) break;
            if (p % q == 0) { ok = false; break; }
        }
        if (!ok) {
            cand += 2;
            continue;
        }
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }
    return pr[(size_t)k - 1];
}

seq_t a(idx_t n)
{
    uint32_t p = A238105_get_prime(n);
    if (p == 2 || p == 5) return (seq_t)0;

    uint64_t rem = 1;
    uint64_t sum = 0;
    do {
        rem *= 10;
        sum += rem / p;
        rem %= p;
    } while (rem != 1);

    return (seq_t)(sum / 3);
}
