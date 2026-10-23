/*
 * A198020  Number of distinct residues of x^n (mod 2*n+1), x=0..2*n.
 * https://oeis.org/A198020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct residues of x^n (mod 2*n+1), x=0..2*n
 * Range:     u8 n <= 132, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Trial division factorisation of a small integer (≤20001). */
static void factor_small(uint64_t m, uint64_t *pr, int *exp, size_t *cnt)
{
    size_t c = 0;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            int e = 0;
            while (m % p == 0)
            {
                m /= p;
                ++e;
            }
            pr[c] = p;
            exp[c] = e;
            ++c;
        }
    }
    if (m > 1)
    {
        pr[c] = m;
        exp[c] = 1;
        ++c;
    }
    *cnt = c;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;                     /* a(0)=1 is defined in OEIS */
    uint64_t M = 2ULL * (uint64_t)n + 1ULL;   /* modulus 2*n+1 */

    /* Factor M into prime powers. */
    uint64_t primes[32];
    int exps[32];
    size_t fac_cnt;
    factor_small(M, primes, exps, &fac_cnt);

    seq_t result = 1;

    for (size_t f = 0; f < fac_cnt; ++f)
    {
        /* Compute the prime power modulus. */
        uint64_t mod = 1;
        for (int e = 0; e < exps[f]; ++e) mod *= primes[f];

        /* visited array for residues modulo this prime power. */
        char *visited = (char *)calloc((size_t)mod, 1);
        if (!visited) abort();   /* allocation failed */

        seq_t distinct = 0;
        for (idx_t x = 0; x < mod; ++x)
        {
            seq_t r = mod_pow((seq_t)x, (uint64_t)n, (seq_t)mod);
            idx_t ri = (idx_t)r;          /* r is < mod */
            if (!visited[ri])
            {
                visited[ri] = 1;
                ++distinct;
            }
        }
        free(visited);

        result *= distinct;            /* product of counts for each prime power */
    }
    return result;
}
