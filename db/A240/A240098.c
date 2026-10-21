/*
 * A240098  Product of the greatest common divisors of n and k! over k=0..n-1.
 * https://oeis.org/A240098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n-1} gcd(n,k!)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 11, u64 n <= 19, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* fast exponentiation for seq_t (works for built‑in types and arbitrary‑precision types). */
static seq_t seq_pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1)=1 */

    /* factorise n into distinct primes and their exponents */
    idx_t primes[64];
    idx_t exps[64];
    size_t pc = 0;

    idx_t temp = n;
    idx_t cnt;

    /* factor 2 separately */
    cnt = 0;
    while ((temp & 1) == 0)
    {
        ++cnt;
        temp >>= 1;
    }
    if (cnt)
    {
        primes[pc] = 2;
        exps[pc] = cnt;
        ++pc;
    }

    /* odd factors */
    for (idx_t p = 3; p * p <= temp; p += 2)
    {
        cnt = 0;
        while (temp % p == 0)
        {
            ++cnt;
            temp /= p;
        }
        if (cnt)
        {
            primes[pc] = p;
            exps[pc] = cnt;
            ++pc;
        }
    }

    if (temp > 1)
    {
        primes[pc] = temp;
        exps[pc] = 1;
        ++pc;
    }

    /* arrays for current exponents in k! and cumulative sum of min(exps, cur) */
    idx_t *cur = (idx_t*)calloc(pc, sizeof(idx_t));
    idx_t *sum = (idx_t*)calloc(pc, sizeof(idx_t));
    if (!cur || !sum)   /* allocation failure guard */
    {
        free(cur);
        free(sum);
        return (seq_t)0;
    }

    /* iterate k = 1 .. n-1 (k=0 gives 0! = 1, contributes nothing) */
    for (idx_t k = 1; k < n; ++k)
    {
        /* update cur with multiplicity of each prime in k */
        for (size_t i = 0; i < pc; ++i)
        {
            idx_t p = primes[i];
            idx_t t = k;
            while (t % p == 0)
            {
                ++cur[i];
                t /= p;
            }
        }

        /* accumulate min(cur[i], exps[i]) for this k */
        for (size_t i = 0; i < pc; ++i)
        {
            idx_t add = cur[i] < exps[i] ? cur[i] : exps[i];
            sum[i] += add;
        }
    }

    /* compute the final product a(n) = Π p^sum[i] */
    seq_t result = 1;
    for (size_t i = 0; i < pc; ++i)
    {
        seq_t pw = seq_pow_seq((seq_t)primes[i], sum[i]); /* p^sum[i] */
        result = result * pw;
    }

    free(cur);
    free(sum);
    return result;
}
