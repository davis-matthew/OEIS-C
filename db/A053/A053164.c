/*
 * A053164  4th root of largest 4th power dividing n.
 * https://oeis.org/A053164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Π_{p^e∥n} p^{⌊e/4⌋}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 128 terms through n = 8712 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ipow_idx.h"

/* ---------- Trial division factorisation ----------------------------------- */
static void _factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    /* factor 2 separately */
    if ((n & 1ULL) == 0) {
        pr[*cnt] = 2ULL;
        exp[*cnt] = 0;
        while ((n & 1ULL) == 0) {
            n >>= 1;
            exp[*cnt] += 1;
        }
        (*cnt)++;
    }
    /* odd primes */
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            pr[*cnt] = d;
            exp[*cnt] = 0;
            while (n % d == 0) {
                n /= d;
                exp[*cnt] += 1;
            }
            (*cnt)++;
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        exp[*cnt] = 1;
        (*cnt)++;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) {
        return (seq_t)1;
    }
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    _factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e4 = exps[i] / 4;
        if (e4 == 0) {
            continue;
        }
        seq_t term = ipow_idx((seq_t)primes[i], e4);
        res *= term;
    }
    return res;
}
