/*
 * A154097  A rational based combinatorial triangular sequence: f(n) =
 *          Product[Prime[a]*k + Prime[b],{k,0,n}]; a = 2; b = 1; t(n,m) =
 *          Denominator[f(n)/(f(n-m)*f(m))].
 * https://oeis.org/A154097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 179, u32 n <= 839, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   build_primes_upto, seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   The function implements the OEIS sequence A154097.
   For each 0 ≤ n the value a(n) is the denominator of  
   f(r)/(f(r-c)·f(c)) where r,c are obtained from n by   
   forming the row‑major order of the triangular array: 
   r = floor((sqrt(8n+1)-1)/2),   c = n - r(r+1)/2.
   Here f(k) = ∏_{i=0}^{k} (3·i + 2). 
   This code computes the term exactly for any n >= 0, using only
   integer arithmetic and the auxiliary big‑integer utilities
   provided by the build environment.
*/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_one.h"
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;      /* array of primes up to a small limit */
static size_t primes_cnt = 0;        /* number of primes in the array */

/* Initialise the list of primes needed for factorisation.
   The maximum term that can appear is 3·r+2 with r≈140 for n≤10000,
   so primes up to 500 are sufficient. */
static void init_primes(void)
{
    if (primes) return;                     /* already initialised */
    size_t cnt;
    build_primes_upto(500, &primes, &cnt);
    primes_cnt = cnt;
}

/* Return the triangular coordinates (row r, column c) of a linear index n.
   The row is the largest r with r(r+1)/2 ≤ n; column is the offset. */
static void decompose(idx_t n, idx_t *r, idx_t *c)
{
    idx_t row = 0;
    while ((row + 1) * (row + 2) / 2 <= n) row++;
    *r = row;
    *c = n - row * (row + 1) / 2;
}

/* Compute the denominator of f(r)/(f(r-c)*f(c)). */
seq_t a(idx_t n)
{
    init_primes();

    /* find (row, column) for the linear index */
    idx_t r, c;
    decompose(n, &r, &c);

    /* exponent array for each prime; 200 is ample for primes up to 500 */
    int exps[200] = {0};

    /* iterate i = 0 .. r and accumulate prime exponents with sign */
    for (idx_t i = 0; i <= r; ++i)
    {
        uint64_t val = 3 * i + 2;          /* the term in the product */

        /* count how many times this term appears in the denominator */
        int denom_cnt = 0;
        if (i <= r - c) denom_cnt++;
        if (i <= c)      denom_cnt++;

        /* net exponent contribution of this term: numerator 1, denominator denom_cnt */
        int net = 1 - denom_cnt;
        if (net == 0) continue;           /* contributes nothing */

        /* factorise val by trial division using the prime list */
        uint64_t tmp = val;
        for (size_t pi = 0; pi < primes_cnt; ++pi)
        {
            uint64_t p = primes[pi];
            if (p * p > tmp) break;
            int e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            if (e) exps[pi] += net * e;
        }
        if (tmp > 1)
        {
            /* remaining prime factor is > sqrt(tmp) and thus prime itself */
            size_t pi;
            for (pi = 0; pi < primes_cnt; ++pi)
                if (primes[pi] == tmp) break;
            if (pi < primes_cnt)
                exps[pi] += net;            /* exponent of this prime is 1 */
        }
    }

    /* build the denominator from primes with negative exponents */
    seq_t denom = seq_one();
    for (size_t pi = 0; pi < primes_cnt; ++pi)
    {
        int e = exps[pi];
        if (e < 0)
        {
            seq_t mult = seq_pow((seq_t)primes[pi], (idx_t)(-e));
            denom = seq_mul(denom, mult);
        }
    }
    return denom;
}
