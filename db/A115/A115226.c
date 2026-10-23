/*
 * A115226  Order of the group of invertible 3 X 3 symmetric matrices over
 *          Z(n).
 * https://oeis.org/A115226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = ∏_{p^e‖n} p^(6e-4) * (p^3-1) * (p-1) , a(1)=1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1626,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* simple trial‑division factorisation (sufficient for n ≤ 10^5) */
static void factor(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t pos = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            uint64_t e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            pr[pos]   = p;
            ex[pos]   = e;
            ++pos;
        }
    }
    if (n > 1)
    {
        pr[pos]   = n;
        ex[pos]   = 1;
        ++pos;
    }
    *cnt = pos;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;

    uint64_t nn = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor(nn, primes, exps, &cnt);

    seq_t total = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        idx_t   exp_pow = 6 * e - 4;

        seq_t powpart  = pow_seq((seq_t)p, exp_pow);
        seq_t p3       = (seq_t)p * (seq_t)p * (seq_t)p;
        seq_t part     = (p3 - 1) * ((seq_t)p - 1);
        seq_t factor_val = powpart * part;
        total *= factor_val;
    }
    return total;
}
