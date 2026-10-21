/*
 * A090330  Number of divisors >1 of n that are prefixes of other divisors of
 *          n in binary representation.
 * https://oeis.org/A090330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors > 1 of n that occur as a binary prefix
 *            of another distinct divisor of n
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Number of bits needed to represent x (x>0). */
static inline int bitlen_u64(uint64_t x)
{
    return x ? 64 - __builtin_clzll(x) : 0;
}

/* Prime factorisation of n (trial division).  The arrays pr[] and exp[]
   must have room for at least 64 factors.  pcnt is the number of distinct
   prime factors found. */
static void factor_u64(uint64_t n, uint64_t *pr, uint64_t *exp, size_t *pcnt)
{
    size_t c = 0;
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
            pr[c]   = p;
            exp[c]  = e;
            ++c;
        }
    }
    if (n > 1)
    {
        pr[c]   = n;
        exp[c]  = 1;
        ++c;
    }
    *pcnt = c;
}

/* Recursive generation of all divisors.  divs[] must be large enough to hold
   all divisors.  cnt is updated to the number of divisors written. */
static void gen_divisors(const uint64_t *pr, const uint64_t *exp,
                         int idx, int pcnt, uint64_t cur,
                         uint64_t *divs, size_t *cnt)
{
    if (idx == pcnt)
    {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t p   = pr[idx];
    uint64_t mult = 1;
    for (uint64_t e = 0; e <= exp[idx]; ++e)
    {
        gen_divisors(pr, exp, idx + 1, pcnt, cur * mult, divs, cnt);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* offset starts at 1 */

    /* 1.  factorise n */
    uint64_t pr[64], exp[64];
    size_t   pcnt;
    factor_u64((uint64_t)n, pr, exp, &pcnt);

    /* 2.  generate all divisors */
    uint64_t divs[4096];
    size_t   div_cnt = 0;
    gen_divisors(pr, exp, 0, (int)pcnt, 1, divs, &div_cnt);

    /* 3.  count divisors that are prefixes of another divisor */
    size_t count = 0;
    for (size_t i = 0; i < div_cnt; ++i)
    {
        uint64_t d = divs[i];
        if (d <= 1) continue;          /* only divisors > 1 */
        int l1 = bitlen_u64(d);
        for (size_t j = 0; j < div_cnt; ++j)
        {
            if (i == j) continue;
            uint64_t d2 = divs[j];
            int l2 = bitlen_u64(d2);
            if (l2 <= l1) continue;    /* need a longer divisor */
            if ((d2 >> (l2 - l1)) == d)
            {
                ++count;
                break;                 /* each divisor counted once */
            }
        }
    }

    return (seq_t)count;
}
