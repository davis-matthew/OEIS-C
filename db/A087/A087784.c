/*
 * A087784  Number of solutions to x^2 + y^2 + z^2 = 1 mod n.
 * https://oeis.org/A087784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = n^2 * (3/2 if 4|n) * ∏_{p≡1 (mod 4)|n} (1+1/p) * ∏_{p≡3
 *            (mod 4)|n} (1-1/p)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 4225, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* simple trial‑division factorisation (works for n up to typical limits) */
static void factorize_u64(uint64_t n,
                          uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    uint64_t t = n;
    if (t % 2 == 0) {
        pr[*cnt] = 2;
        exp[*cnt] = 0;
        while (t % 2 == 0) { exp[*cnt]++; t /= 2; }
        ++(*cnt);
    }
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            pr[*cnt] = p;
            exp[*cnt] = 0;
            while (t % p == 0) { exp[*cnt]++; t /= p; }
            ++(*cnt);
        }
    }
    if (t > 1) {
        pr[*cnt] = t;
        exp[*cnt] = 1;
        ++(*cnt);
    }
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1U;

    seq_t result = (seq_t)n * (seq_t)n; /* n² */

    /* factor 3/2 if 4|n */
    if ((n & 3U) == 0) {
        result = result * (seq_t)3 / (seq_t)2;
    }

    /* factorisation of n */
    uint64_t pr[32], exp[32];
    size_t cnt;
    factorize_u64((uint64_t)n, pr, exp, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        if ((p & 3U) == 1) {          /* p ≡ 1 (mod 4) */
            result = result * (seq_t)(p + 1);
            result = result / (seq_t)p;
        } else if ((p & 3U) == 3) {   /* p ≡ 3 (mod 4) */
            result = result * (seq_t)(p - 1);
            result = result / (seq_t)p;
        }
        /* p ≡ 2 (mod 4) → no adjustment */
    }

    return result;
}
