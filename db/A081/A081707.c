/*
 * A081707  a(n) = tau(n) - bigomega(n) = A000005(n) - A001222(n).
 * https://oeis.org/A081707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(n) – Ω(n) (number of divisors minus total number of
 *            prime factors)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9433 (2026-11-01)
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
    if (n < 1) return 0;          /* offset is 1, behaviour below is irrelevant */

    /* local buffers for primes and exponents (64 is ample for 64‑bit integers) */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt = 0;

    factor((uint64_t)n, pr, exp, &cnt);   /* factorise n */

    seq_t tau   = 1;   /* τ(n)  = Π (exp[i] + 1) */
    seq_t omega = 0;   /* Ω(n)  = Σ exp[i]          */

    for (size_t i = 0; i < cnt; ++i)
    {
        tau   *= (seq_t)(exp[i] + 1);
        omega += (seq_t)exp[i];
    }

    return tau - omega;
}
