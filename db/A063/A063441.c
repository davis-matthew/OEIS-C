/*
 * A063441  a(n) = sigma(n) * mu(n).
 * https://oeis.org/A063441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign, mult
 * Formula:   a(n) = sigma(n) * mu(n)
 * Range:     i8 n <= 12, i16 n <= 777, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma(n) = sum of divisors, mu(n) = Möbius function  */
/* Computed by trial division of n, suitable for all n ≥ 1 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* small exponentiation for seq_t */
static inline seq_t seq_pow_seq(seq_t base, int exp)
{
    seq_t res = 1;
    for (int i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    /* a(1) = sigma(1)*mu(1) = 1*1 = 1 */
    if (n == 1) return 1;

    /* prime factors and exponents */
    idx_t primes[64];
    int    exps [64];
    size_t cnt = 0;

    idx_t rem = n;

    /* factor 2 */
    if ((rem & 1ULL) == 0) {
        int e = 0;
        while ((rem & 1ULL) == 0) {
            rem >>= 1;
            ++e;
        }
        if (e > 1) return 0;            /* μ(n)=0 for non‑squarefree */
        primes[cnt] = 2ULL;
        exps[cnt]   = e;
        ++cnt;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= rem; p += 2) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            if (e > 1) return 0;        /* μ(n)=0 for non‑squarefree */
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }

    /* remaining prime > sqrt(n) */
    if (rem > 1) {
        primes[cnt] = rem;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* compute σ(n) = Π (p^(e+1)-1)/(p-1) */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p_seq = (seq_t)primes[i];
        seq_t term  = (seq_pow_seq(p_seq, exps[i] + 1) - 1) / (p_seq - 1);
        sigma *= term;
    }

    /* μ(n) = (-1)^cnt (cnt > 0 here) */
    seq_t sign = (cnt & 1) ? -1 : 1;

    return sigma * sign;
}
