/*
 * A175781  a(n) = n^(1/k) with the smallest k>1 such that n is a k-th power,
 *          taking k=1 if no such k>1 exists.
 * https://oeis.org/A175781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computation of OEIS sequence A175781:
   a(n) = n^(1/k) where k is the smallest integer > 1 such that n is a k‑th power.
   If no such k exists, a(n) = n.
*/
#include <stdint.h>
#include <stdbool.h>

/* helper: Euclidean gcd for integers */
static int gcd_int(int a, int b)
{
    while (b != 0)
    {
        int t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* helper: integer power with seq_t result (supports unsigned __int128, big ints, etc.) */
static seq_t int_pow_uint(uint64_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp != 0)
    {
        if (exp & 1u)
            result *= cur;
        exp >>= 1u;
        if (exp)
            cur *= cur;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n is at least 1 (OEIS offset) */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    uint64_t tmp = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    int count = 0;

    /* trial division factorization */
    for (uint64_t d = 2; d * d <= tmp; ++d)
    {
        if (tmp % d == 0)
        {
            int e = 0;
            while (tmp % d == 0)
            {
                tmp /= d;
                ++e;
            }
            primes[count]   = d;
            exps[count]     = e;
            ++count;
        }
    }
    if (tmp > 1)
    {
        primes[count]   = tmp;
        exps[count]     = 1;
        ++count;
    }

    /* find gcd of exponents */
    int g = exps[0];
    for (int i = 1; i < count; ++i)
        g = gcd_int(g, exps[i]);

    /* if g==1, no higher power exists */
    if (g == 1)
        return (seq_t)n;

    /* smallest divisor > 1 of g */
    int k = 0;
    for (int d = 2; d * d <= g; ++d)
    {
        if (g % d == 0)
        {
            k = d;
            break;
        }
    }
    if (k == 0)
        k = g;          /* g itself is prime */

    /* compute the k-th root via prime exponents */
    seq_t result = 1;
    for (int i = 0; i < count; ++i)
    {
        int e_div = exps[i] / k;             /* guaranteed integer division */
        seq_t part = int_pow_uint(primes[i], (idx_t)e_div);
        result *= part;
    }
    return result;
}
