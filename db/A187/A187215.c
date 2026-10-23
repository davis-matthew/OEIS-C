/*
 * A187215  Sum of the elements of the absolute difference table of the
 *          divisors of n.
 * https://oeis.org/A187215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of all elements of the absolute difference triangle
 *            of the divisors of n
 * Range:     u8 n <= 53, u16 n <= 6434, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function factors n, generates and sorts its divisors, then repeatedly
   replaces the current row by the absolute differences of adjacent elements
   while adding every row to the total sum.  */
#include <stdint.h>
#include <stdlib.h>

/* Simple trial‑division factorization of a 64‑bit number. */
static void factor_uint64(uint64_t n, uint64_t *primes, int *exps, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p)
    {
        if (n % p == 0)
        {
            int e = 0;
            while (n % p == 0)
            {
                n /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (n > 1)
    {
        primes[c] = n;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* Recursively generate all divisors from the prime factorization. */
static void gen_divisors(size_t fi, uint64_t cur, uint64_t *out, size_t *pos,
                         const uint64_t *pf, const int *ep, size_t pcnt)
{
    if (fi == pcnt)
    {
        out[(*pos)++] = cur;
        return;
    }
    uint64_t p = pf[fi];
    uint64_t val = cur;
    for (int e = 0; e <= ep[fi]; ++e)
    {
        gen_divisors(fi + 1, val, out, pos, pf, ep, pcnt);
        val *= p;
    }
}

/* Comparator for qsort. */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    uint64_t u = (uint64_t)n;
    uint64_t primes[64];
    int exps[64];
    size_t pcnt = 0;
    factor_uint64(u, primes, exps, &pcnt);

    /* Count total number of divisors. */
    size_t cnt = 1;
    for (size_t i = 0; i < pcnt; ++i)
        cnt *= (size_t)(exps[i] + 1);

    uint64_t *divs = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!divs)
        return 0;

    size_t pos = 0;
    gen_divisors(0, 1ULL, divs, &pos, primes, exps, pcnt);

    qsort(divs, cnt, sizeof(uint64_t), cmp_uint64);

    seq_t sum = 0;
    size_t len = cnt;
    while (len > 0)
    {
        for (size_t i = 0; i < len; ++i)
            sum += (seq_t)divs[i];

        if (len == 1)
            break;

        /* Next row: absolute differences of adjacent elements. */
        for (size_t i = 0; i + 1 < len; ++i)
        {
            uint64_t a = divs[i];
            uint64_t b = divs[i + 1];
            divs[i] = (b >= a) ? (b - a) : (a - b);
        }
        --len;
    }

    free(divs);
    return sum;
}
