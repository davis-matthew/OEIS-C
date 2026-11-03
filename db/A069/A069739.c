/*
 * A069739  Size of the key space for isomorphism verification of circulant
 *          graphs of order n.
 * https://oeis.org/A069739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = ∏_{p^m‖n} Catalan(m) (Catalan numbers = A000108)
 * Range:     u8 n <= 127, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 106 terms through n = 9447 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------- */
static void factor(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p*p <= n; ++p) {
        if (n % p == 0) {
            pr[c] = p; exp[c] = 0;
            while (n % p == 0) { n /= p; ++exp[c]; }
            ++c;
        }
    }
    if (n > 1) { pr[c] = n; exp[c] = 1; ++c; }
    *cnt = c;
}

/* Compute the m‑th Catalan number.  m≥0. */
static inline seq_t catalan(idx_t m)
{
    if (m == 0) return 1;
    seq_t res = 1;
    for (idx_t i = 1; i <= m; ++i) {
        /* C_m = C_{m-1} * 2*(2*i-1)/(i+1)   (exact division) */
        seq_t num = (seq_t)(2 * (2 * i - 1));
        seq_t den = (seq_t)(i + 1);
        res = res * num / den;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1)=1, and a(0) is unused but safe */
        return 1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor(nn, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
        result = result * catalan((idx_t)exps[i]);

    return result;
}
