/*
 * A066858  Smallest k>1 such that for each integer x, x^k=x or x^k=0 (mod n);
 *          or 0 if no such k exists.
 * https://oeis.org/A066858
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k>1 such that for all integers x, x^k ≡ x
 *            or x^k ≡ 0 (mod n); returns 0 if no such k exists. The
 *            implementation factors n, checks if it is either a prime power
 *            or a squarefree integer. For such n it returns Carmichael(n)+1,
 *            otherwise 0
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static seq_t lcm_seq(seq_t a, seq_t b)
{
    return (a / gcd(a, b)) * b;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined for n<2 but safe */

    uint64_t nn   = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt   = 0;

    factor_u64(nn, primes, exps, &cnt);

    /* Detect prime power or squarefree */
    int  all_one = 1;             /* flag: all exponents == 1 */
    int  more_than_one = 0;       /* true if any exponent > 1 */

    for (size_t i = 0; i < cnt; ++i)
    {
        if (exps[i] > 1) { all_one = 0; more_than_one = 1; }
    }

    if (cnt == 1)    /* prime power (including primes) */
    {
        uint64_t p = primes[0];
        seq_t    lambda;

        if (p == 2)
        {
            if (exps[0] == 1)            lambda = 1;
            else if (exps[0] == 2)      lambda = 2;
            else                        lambda = ((seq_t)1 << (exps[0] - 2));  /* 2^{a-2} */
        }
        else
        {
            /* Carmichael for odd prime powers: λ(p^a) = φ(p^a) = p^{a-1}*(p-1) */
            seq_t pow = 1;
            for (uint64_t i = 0; i < exps[0] - 1; ++i) pow *= (seq_t)p;
            lambda = pow * ((seq_t)p - 1);
        }
        return lambda + 1;
    }
    else if (all_one)   /* squarefree composite */
    {
        seq_t l = 1;
        for (size_t i = 0; i < cnt; ++i)
        {
            seq_t val = (seq_t)primes[i] - 1;
            l = lcm_seq(l, val);
        }
        return l + 1;
    }
    else                /* not squarefree and not prime power */
    {
        return 0;
    }
}
