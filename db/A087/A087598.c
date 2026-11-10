/*
 * A087598  Numbers m such that all terms in the sequence m, A040115(m),
 *          A040115(A040115(m)), ..., 0 are triangular numbers (A000217).
 * https://oeis.org/A087598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, full, fini
 * Formula:   a(n) = n-th number m (m = 0, 1, 3, ...) such that m, A040115(m),
 *            A040115(A040115(m)), ..., 0 are all triangular. Computed by a
 *            search over triangular numbers; the sequence is finite with 20
 *            terms, so a(n) = 0 for n > 20
 * Range:     u8 n <= 13, u16 n <= 19, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t A040115_step(uint64_t n)
{
    if (n < 10u) return 0;
    uint8_t buf[20];
    size_t len = 0;
    while (n) { buf[len++] = (uint8_t)(n % 10u); n /= 10u; }
    uint64_t result = 0;
    bool seen = false;
    for (size_t i = len; i > 1; --i) {
        int d1 = buf[i - 1], d2 = buf[i - 2];
        int diff = d1 > d2 ? d1 - d2 : d2 - d1;
        if (!seen) {
            if (diff == 0) continue;
            seen = true;
        }
        result = result * 10u + (uint64_t)diff;
    }
    return result;
}

static bool A087598_is_tri(uint64_t t)
{
    uint64_t v = 8u * t + 1u;
    uint64_t r = 0;
    uint64_t lo = 0, hi = 4000000000ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= v) lo = mid; else hi = mid - 1;
    }
    r = lo;
    return r * r == v;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint64_t count = 0;
    for (uint64_t k = 0; k <= 2000000u; ++k) {
        uint64_t t = k * (k + 1) / 2;
        uint64_t x = t;
        bool ok = true;
        while (x > 0) {
            x = A040115_step(x);
            if (!A087598_is_tri(x)) { ok = false; break; }
        }
        if (ok) {
            ++count;
            if (count == (uint64_t)n) return (seq_t)t;
        }
    }
    return (seq_t)0;
}
