/*
 * A064547  Sum of binary digits (or count of 1-bits) in the exponents of the
 *          prime factorization of n.
 * https://oeis.org/A064547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum of popcount(e_i) over the exponents e_i in the prime
 *            factorisation of n (a(1)=0)
 * Range:     u8 n <= 32768, u16 n <= 32768, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 121 terms through n = 9487 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
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

seq_t a(idx_t n)
{
    if (n == 1)                 /* a(1) = 0 by definition */
        return (seq_t)0;

    uint64_t m = (uint64_t)n;   /* work in 64‑bit arithmetic */

    uint64_t pr[64];            /* array for prime factors (unlikely > 64) */
    uint64_t exp[64];           /* corresponding exponents          */
    size_t   cnt = 0;           /* number of distinct prime factors */

    factor(m, pr, exp, &cnt);   /* factorise n */

    seq_t sum = 0;              /* result accumulator */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exp[i];
        seq_t pop = 0;          /* popcount of exponent e          */

        while (e) {
            pop += e & 1;
            e >>= 1;
        }
        sum += pop;
    }

    return sum;
}
