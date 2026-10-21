/*
 * A096226  a(n) is the least exponent k > 1 such that m^k is congruent to m
 *          modulo n for all natural numbers m, or a(n) = 1 if no such k
 *          exists.
 * https://oeis.org/A096226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + lcm(p1-1, p2-1, ..., pj-1) if n = p1*p2*...*pj is
 *            square‑free a(n) = 1 otherwise (including n=1 gives 2) Since by
 *            convention lcm of an empty set is 1, a(1)=1+1=2
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"

/* ------------------------------------------------------------------ */
/* Simple factorisation: trial division (sufficient for n ≤ 10 000).   */
/* ------------------------------------------------------------------ */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t k = 0;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            uint64_t e = 0;
            while (n % d == 0) {
                n /= d;
                ++e;
            }
            pr[k]   = d;
            exp[k]  = e;
            ++k;
        }
        ++d;
    }
    if (n > 1) {          /* remaining prime factor */
        pr[k]  = n;
        exp[k] = 1;
        ++k;
    }
    *cnt = k;
}

/* ------------------------------------------------------------------ */
/* Euclidean GCD for 64‑bit values.                                   */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Sequence A096226.                                                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Convert index to integer (domain n ≥ 1). */
    uint64_t m = (uint64_t)n;
    if (m == 0) return (seq_t)0;     /* not used in OEIS, but safe */

    /* Factorise m.  64-bit arrays are ample for n up to 10 000. */
    uint64_t pr[64];
    uint64_t exp[64];
    size_t   cnt;
    factor_u64(m, pr, exp, &cnt);

    /* If m=1, cnt==0 => lcm stays 1 and answer 2. */
    uint64_t lcm = 1;
    for (size_t i = 0; i < cnt; ++i) {
        if (exp[i] > 1)            /* not square‑free → answer 1 */
            return (seq_t)1;
        uint64_t p_minus = pr[i] - 1;
        uint64_t g = gcd_u64(lcm, p_minus);
        lcm = (lcm / g) * p_minus;
    }
    /* For square‑free n: a(n) = 1 + lcm(p_i-1).  For n=1, lcm==1 → 2. */
    return (seq_t)(lcm + 1);
}
