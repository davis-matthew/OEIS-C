/*
 * A065960  a(n) = n^4*Product_{distinct primes p dividing n} (1+1/p^4).
 * https://oeis.org/A065960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^4 * Π_{p|n,distinct} (1 + 1/p^4) Implementation: factor
 *            n, compute a(p^e) = p^(4e) + p^(4e-4) and multiply over all
 *            prime powers
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 251, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor.h"

/* Simple integer exponentiation for sequence type */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* Factor n: prime factors and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor((uint64_t)n, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t p_sext = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];
        idx_t exp4 = e << 2;            /* 4 * e */
        seq_t pow4e = pow_seq(p_sext, exp4);
        seq_t pow4e_minus4;
        if (exp4 >= 4)
            pow4e_minus4 = pow_seq(p_sext, exp4 - 4);
        else
            pow4e_minus4 = (seq_t)1;    /* For e == 1, 4*e-4 == 0 */
        seq_t term = pow4e + pow4e_minus4;
        res *= term;
    }
    return res;
}
