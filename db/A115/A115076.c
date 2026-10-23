/*
 * A115076  Number of 2 X 2 symmetric matrices over Z(n) having determinant 1.
 * https://oeis.org/A115076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = product over prime powers p^e || n of f(p,e) where f(2,1)
 *            = 4, f(2,e) = 3 * 2^(2e-2) (for e>1), f(p,e) = (p+1) * p^(2e-1)
 *            for p ≡ 1 (mod 4), f(p,e) = (p-1) * p^(2e-1) for p ≡ 3 (mod 4)
 * Range:     u8 n <= 16, u16 n <= 249, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The following implementation uses only the C language
   features available on all supported builds.  */
#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */

/*------------------------------------------------------------------*/
/* Utility: fast integer power in seq_t arithmetic                    */
/*------------------------------------------------------------------*/
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/*------------------------------------------------------------------*/
/* Utility: trial‑division factorisation of n (works for n≤UINT64_MAX)*/
/*------------------------------------------------------------------*/
static void factor_uint64(uint64_t n, uint64_t primes[], int exps[], size_t *cnt)
{
    size_t fcnt = 0;
    uint64_t d;
    for (d = 2; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            int e = 0;
            while (n % d == 0)
            {
                n /= d;
                ++e;
            }
            primes[fcnt] = d;
            exps[fcnt] = e;
            ++fcnt;
        }
    }
    if (n > 1)          /* remaining prime factor */
    {
        primes[fcnt] = n;
        exps[fcnt] = 1;
        ++fcnt;
    }
    *cnt = fcnt;
}

/*------------------------------------------------------------------*/
/* Main sequence function                                           */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 1U)
        return 1;   /* a(1) = 1 by definition */

    /* factor n */
    uint64_t p[64];
    int e[64];
    size_t cnt;
    factor_uint64((uint64_t)n, p, e, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t part;
        if (p[i] == 2ULL)
        {
            if (e[i] == 1)
                part = 4; /* a(2) = 4 */
            else
            {
                /* 3 * 2^(2e-2) */
                seq_t pow2 = pow_seq_t(2, (idx_t)(2 * e[i] - 2));
                part = 3 * pow2;
            }
        }
        else
        {
            /* (p+1 or p-1) * p^(2e-1) depending on p mod 4 */
            seq_t pref = (p[i] % 4ULL == 1ULL) ? (seq_t)(p[i] + 1UL)
                                               : (seq_t)(p[i] - 1UL);
            seq_t powp = pow_seq_t((seq_t)p[i], (idx_t)(2 * e[i] - 1));
            part = pref * powp;
        }

        result *= part;
    }
    return result;
}
