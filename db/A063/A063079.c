/*
 * A063079  Bisection of A001790.
 * https://oeis.org/A063079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = odd part of C(4n-2, 2n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) is the binomial coefficient (4n-2 choose 2n-1) with all factors of 2 removed */

#include <stdint.h>
#include <stdlib.h>

static uint32_t *spf = NULL;
static size_t spf_limit = 0;

static void init_spf(size_t limit)
{
    /* allocate or enlarge spf array */
    if (spf_limit < limit)
    {
        uint32_t *new_spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
        for (size_t i = 0; i <= limit; ++i) new_spf[i] = (uint32_t)i;
        for (size_t i = 2; i * i <= limit; ++i)
        {
            if (new_spf[i] == (uint32_t)i)
            {
                for (size_t j = i * i; j <= limit; j += i)
                    if (new_spf[j] == (uint32_t)j) new_spf[j] = (uint32_t)i;
            }
        }
        free(spf);
        spf = new_spf;
        spf_limit = limit;
    }
}

static void factor_into_exponents(size_t value, int *exp_table, int sign)
{
    /* factor 'value' and add (sign>0) or subtract (sign<0) exponents to exp_table */
    while (value > 1)
    {
        uint32_t p = spf[value];
        int cnt = 0;
        while (value % p == 0)
        {
            value /= p;
            ++cnt;
        }
        exp_table[p] += sign * cnt;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n >= 1 but guard anyway */

    idx_t N = 4 * n - 2;            /* upper term of the binomial */
    idx_t K = 2 * n - 1;            /* lower term (equals N-K) */

    size_t limit = (size_t)N;
    init_spf(limit);

    /* allocation of exponent table (index by prime) */
    int *exp = (int*)calloc(limit + 1, sizeof(int));
    if (!exp) return 0;            /* allocation failure, should not happen */

    /* accumulate exponents from numerators and denominators */
    for (idx_t k = 1; k <= K; ++k)
    {
        size_t num = (size_t)(K + k);   /* N-K + k = K + k */
        size_t den = (size_t)k;

        factor_into_exponents(num, exp, +1);
        factor_into_exponents(den, exp, -1);
    }

    /* build the odd part: multiply all primes except 2 according to exponents */
    seq_t result = 1;
    for (size_t p = 3; p <= limit; p += 2)   /* skip the prime 2 */
    {
        int e = exp[p];
        while (e-- > 0)
            result *= (seq_t)p;
    }

    free(exp);
    return result;
}
