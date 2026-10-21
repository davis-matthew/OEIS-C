/*
 * A067620  a(n) = p^e, where p and e are the rounded means of the prime
 *          factors p_i and the exponents e_i, respectively, in the
 *          factorization n = p_1^e_1 * ... * p_r^e_r of n into distinct
 *          primes p_i. Each mean is rounded to the nearest integer, rounding
 *          up if there's a choice.
 * https://oeis.org/A067620
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = p^e, where p is the nearest integer to the mean of the
 *            distinct prime factors of n, and e is the nearest integer to the
 *            mean of the exponents in the prime‑factorisation of n. Ties are
 *            rounded up. Offset: index 2 (a(2) is the first term)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   round_mean, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/round_mean.h"
#include "../utils/seq_mul.h"

/* Multiply two seq_t values using ordinary unsigned arithmetic.
   Overflow is modulo 2^N, which is fine for unsigned types. */

/* Fast exponentiation: base^exp */
static seq_t seq_pow(seq_t base, unsigned long long exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1ULL) result = seq_mul(result, base);
        base = seq_mul(base, base);
        exp >>= 1ULL;
    }
    return result;
}

/* Simple trial‑division factorisation.
   Stores distinct prime factors in pr[64] and their exponents in exp[64].
   cnt is set to the number of distinct prime factors. */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    *cnt = 0;
    if (n < 2ULL) return;

    for (uint64_t d = 2ULL; d * d <= n; ++d) {
        if (n % d == 0ULL) {
            uint64_t e = 0ULL;
            while (n % d == 0ULL) {
                n /= d;
                ++e;
            }
            pr[*cnt] = d;
            exp[*cnt] = e;
            ++(*cnt);
        }
    }
    if (n > 1ULL) {          /* n itself is prime */
        pr[*cnt] = n;
        exp[*cnt] = 1ULL;
        ++(*cnt);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2ULL) return (seq_t)0;   /* undefined for indices below offset */

    uint64_t u = (uint64_t)n;
    uint64_t pr[64];
    uint64_t exp[64];
    size_t cnt = 0;

    factor_uint64(u, pr, exp, &cnt);

    if (cnt == 0) return (seq_t)u;   /* should not happen for n >= 2 */

    unsigned long long sumP = 0ULL;
    unsigned long long sumE = 0ULL;
    for (size_t i = 0; i < cnt; ++i) {
        sumP += pr[i];
        sumE += exp[i];
    }

    /* Compute the rounded means. */
    uint64_t p = (uint64_t)round_mean(sumP, cnt);
    uint64_t e = (uint64_t)round_mean(sumE, cnt);

    return seq_pow((seq_t)p, e);
}
