/*
 * A194532  Jordan function ratio J_6(n)/J_2(n).
 * https://oeis.org/A194532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} p^(4·(e−1)) · (p²+p+1) · (p²−p+1)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 233, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 1.  This implements the multiplicative definition given in OEIS
    A194532.  It works for all n ≥ 1 and uses only integer arithmetic. */

#include <stdint.h>
#include <stdbool.h>

/* helper: exponentiation by squaring, all working in seq_t */
static seq_t pow_seq(seq_t base, unsigned int exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1U) result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; undefined for n < 1, but we guard against it */
    if (n == 0) return (seq_t)0;

    uint64_t m = (uint64_t)n;        /* work in native 64‑bit values */
    seq_t   res = (seq_t)1;

    /* factor 2 separately */
    unsigned int e = 0;
    while ((m & 1U) == 0U)
    {
        m >>= 1U;
        ++e;
    }
    if (e > 0)
    {
        seq_t p = (seq_t)2U;
        seq_t p2 = p * p;                           /* p² */
        seq_t part = (p2 + p + (seq_t)1) * (p2 - p + (seq_t)1);
        seq_t pow_p = pow_seq(p, 4U * (e - 1U));     /* p^(4(e‑1)) */
        res *= pow_p * part;
    }

    /* factor odd primes */
    for (uint64_t i = 3U; i * i <= m; i += 2U)
    {
        if (m % i != 0U) continue;
        e = 0;
        while (m % i == 0U)
        {
            m /= i;
            ++e;
        }
        seq_t p = (seq_t)i;
        seq_t p2 = p * p;
        seq_t part = (p2 + p + (seq_t)1) * (p2 - p + (seq_t)1);
        seq_t pow_p = pow_seq(p, 4U * (e - 1U));
        res *= pow_p * part;
    }

    /* if a prime factor remains > 1 */
    if (m > 1U)
    {
        seq_t p = (seq_t)m;
        seq_t p2 = p * p;
        seq_t part = (p2 + p + (seq_t)1) * (p2 - p + (seq_t)1);
        /* e == 1, so p^(4*(1‑1)) == 1 */
        res *= part;
    }

    return res;
}
