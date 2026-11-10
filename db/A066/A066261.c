/*
 * A066261  a(n) = A066260(A066260(n)).
 * https://oeis.org/A066261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A066260(A066260(n)), A066260 completely multiplicative:
 *            prime p_k -> k-th composite; computed via factorization of n and
 *            of each image
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 62 terms through n = 972 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A066261_isprime(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return false;
    return true;
}

/* index of prime p (1-based) */
static uint64_t A066261_pidx(uint64_t p)
{
    uint64_t c = 0;
    for (uint64_t m = 2; m <= p; ++m)
        if (A066261_isprime(m)) ++c;
    return c;
}

/* k-th composite, k >= 1 */
static uint64_t A066261_comp(uint64_t k)
{
    uint64_t m = 3, c = 0;
    while (c < k) {
        ++m;
        if (!A066261_isprime(m)) ++c;
    }
    return m;
}

/* f(f(p)) for prime p */
static seq_t A066261_g(uint64_t p)
{
    uint64_t c = A066261_comp(A066261_pidx(p));
    seq_t g = 1;
    for (uint64_t q = 2; q <= c; ++q) {
        if (c % q != 0) continue;
        uint64_t c2 = A066261_comp(A066261_pidx(q));
        while (c % q == 0) {
            c /= q;
            g *= (seq_t)c2;
        }
    }
    return g;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t r = (uint64_t)n;
    for (uint64_t p = 2; p <= r; ++p) {
        if (r % p != 0) continue;
        seq_t g = A066261_g(p);
        while (r % p == 0) {
            r /= p;
            result *= g;
        }
    }
    return result;
}
