/*
 * A069546  a(n) = Sum_{d|n} sigma(n*d).
 * https://oeis.org/A069546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, easy, nonn
 * Formula:   a(n) = Sum_{d|n} σ(n*d). * Multiplicative: a(p^e) =
 *            (p^(e+1)*(p^(e+1)-1) - (p-1)*(e+1))/(p-1)^2
 * Range:     u8 n <= 9, u16 n <= 95, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* fast exponentiation: base^exp  (exp is 0‑based, returns seq_t) */
static seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    uint64_t b = (uint64_t)base;
    uint64_t r = 1;
    while (exp) {
        if (exp & 1) r *= b;
        b *= b;
        exp >>= 1;
    }
    return (seq_t)r;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0 */

    uint64_t nn = (uint64_t)n;
    uint64_t pr[32];          /* primes */
    uint64_t ex[32];          /* exponents */
    size_t cnt;
    factor_u64(nn, pr, ex, &cnt);   /* factor n */

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = pr[i];
        uint64_t e   = ex[i];
        seq_t   te   = pow_seq_t((seq_t)p, e + 1);          /* p^(e+1) */
        uint64_t t1  = (uint64_t)te;
        uint64_t num = t1 * (t1 - 1) - (uint64_t)(p - 1) * (e + 1);
        uint64_t den = (uint64_t)(p - 1) * (p - 1);
        seq_t   term = (seq_t)(num / den);                  /* exact division */
        result *= term;
    }
    return result;
}
