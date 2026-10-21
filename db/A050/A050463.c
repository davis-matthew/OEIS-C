/*
 * A050463  a(n) = Sum_{d|n, n/d=1 mod 4} d^4.
 * https://oeis.org/A050463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, (n/d) ≡ 1 (mod 4)} d⁴
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---- necessary includes ------------------------------------------- */
#include <stdint.h>
#include <stddef.h>

/* ---- factorisation of a 64‑bit integer -------------------------------- */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    uint64_t e;

    if (n == 0) {
        *cnt = 0;
        return;
    }

    /* factor 2 */
    e = 0;
    while ((n & 1ULL) == 0) {
        n >>= 1;
        e++;
    }
    if (e > 0) {
        pr[c] = 2;
        exp[c++] = e;
    }

    /* odd factors */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            pr[c] = p;
            exp[c++] = e;
        }
    }

    /* remaining prime > 1 */
    if (n > 1) {
        pr[c] = n;
        exp[c++] = 1;
    }

    *cnt = c;
}

/* ---- recursive enumeration of divisors ------------------------------ */
static void enumerate_divs(const uint64_t *pr, const uint64_t *exp,
                           size_t cnt, size_t idx,
                           uint64_t cur, uint64_t n, seq_t *sum)
{
    if (idx == cnt) {
        uint64_t d = cur;
        /* n/d is an integer because d|n */
        if (((n / d) & 3ULL) == 1ULL) {
            seq_t term = (seq_t)d;
            term *= term;   /* d² */
            term *= term;   /* d⁴ */
            *sum += term;
        }
        return;
    }

    uint64_t p = pr[idx];
    uint64_t val = 1;

    for (uint64_t e = 0; e <= exp[idx]; ++e) {
        enumerate_divs(pr, exp, cnt, idx + 1, cur * val, n, sum);
        val *= p;
    }
}

/* ---- required entry point ------------------------------------------- */
seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t pr[32], exp[32];
    size_t cnt;

    factor_uint64(nn, pr, exp, &cnt);

    seq_t sum = 0;
    enumerate_divs(pr, exp, cnt, 0, 1, nn, &sum);
    return sum;
}
