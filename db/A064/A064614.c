/*
 * A064614  Exchange 2 and 3 in the prime factorization of n.
 * https://oeis.org/A064614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A064614 (Exchange 2 and 3 in the prime factorization of n) */

/* If you need the helper function factor_uint64 you can find it below.
   It performs trial division using 64‑bit arithmetic.  The function
   stores the unique prime factors in `pr`, their exponents in `exp`,
   and the number of distinct factors in `cnt`.  The routine works
   for all n up to 2⁶⁴−1. */

#include <stdint.h>
#include <stddef.h>  /* for size_t */

/* Basic trial division factorisation routine (works for 64‑bit n). */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *cnt)
{
    size_t c = 0;
    if (n <= 1) {
        *cnt = 0;
        return;
    }

    /* factor 2 */
    while ((n & 1ULL) == 0) {
        if (c == 0 || pr[c - 1] != 2ULL) {
            pr[c] = 2ULL;
            exp[c] = 1ULL;
            c++;
        } else {
            exp[c - 1]++;
        }
        n >>= 1;
    }

    /* odd factors */
    for (uint64_t f = 3ULL; f * f <= n; f += 2ULL) {
        while (n % f == 0U) {
            if (c == 0 || pr[c - 1] != f) {
                pr[c] = f;
                exp[c] = 1ULL;
                c++;
            } else {
                exp[c - 1]++;
            }
            n /= f;
        }
    }

    /* remaining prime */
    if (n > 1ULL) {
        if (c == 0 || pr[c - 1] != n) {
            pr[c] = n;
            exp[c] = 1ULL;
            c++;
        } else {
            exp[c - 1]++;
        }
    }

    *cnt = c;
}

/* Main sequence routine.  It is completely multiplicative and may be
   called in any order.  The function uses only integer arithmetic
   and works for any seq_t type that supports +, *, etc. */
seq_t a(idx_t n)
{
    if (n == 1ULL)
        return (seq_t)1;

    uint64_t num = (uint64_t)n;
    uint64_t pr[32];
    uint64_t exp[32];
    size_t cnt;

    factor_uint64(num, pr, exp, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base;
        if (pr[i] == 2ULL)
            base = (seq_t)3;        /* 2 ↦ 3 */
        else if (pr[i] == 3ULL)
            base = (seq_t)2;        /* 3 ↦ 2 */
        else
            base = (seq_t)pr[i];    /* other primes unchanged */

        uint64_t e = exp[i];
        /* multiply `result` by base^e */
        seq_t power = (seq_t)1;
        for (uint64_t j = 0; j < e; ++j)
            power *= base;

        result *= power;
    }

    return result;
}
