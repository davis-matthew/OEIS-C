/*
 * A089819  Number of subsets of {1,2,...,n} containing no primes.
 * https://oeis.org/A089819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n - pi(n))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 45, u64 n <= 86, u128 n <= 165,
 *            big any n
 * Helpers:   pow2idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2idx.h"

/* ------------------------------------------------------------ */
/*  Prime sieve helpers                                          */
/* ------------------------------------------------------------ */
static bool     *prime_flag = NULL;         /* prime_flag[i] == true  if i is prime  */
static idx_t    *prime_pi   = NULL;         /* prime_pi[i]   = #primes ≤ i            */
static idx_t      sieve_max = 0;             /* largest n for which the cache is valid */

static void ensure_sieve(idx_t n)
{
    if (n <= sieve_max)                     /* already valid   */
        return;

    size_t sz = (size_t)(n + 1);

    bool *pf = (bool *)realloc(prime_flag, sz * sizeof(bool));
    if (!pf) exit(1);
    prime_flag = pf;

    idx_t *pi = (idx_t *)realloc(prime_pi, sz * sizeof(idx_t));
    if (!pi) exit(1);
    prime_pi = pi;

    for (idx_t i = 0; i <= n; ++i)          /* initialise as prime */
        prime_flag[i] = true;
    if (n >= 0) prime_flag[0] = false;
    if (n >= 1) prime_flag[1] = false;

    /* Eratosthenes sieve */
    for (idx_t p = 2; (idx_t)(p * p) <= n; ++p)
        if (prime_flag[p])
            for (idx_t j = p * p; j <= n; j += p)
                prime_flag[j] = false;

    /* Prefix counts of primes */
    prime_pi[0] = 0;
    for (idx_t i = 1; i <= n; ++i)
        prime_pi[i] = prime_pi[i-1] + (prime_flag[i] ? 1 : 0);

    sieve_max = n;
}

/* ------------------------------------------------------------ */
/*  Compute 2^exp where exp is an idx_t                         */

/* ------------------------------------------------------------ */
/*  Main sequence function                                       */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)                 /* offset is 1; n<1 is unused */
        return (seq_t)1;       /* harmless default       */

    ensure_sieve(n);
    idx_t pi_n = prime_pi[n];   /* #primes ≤ n             */
    idx_t exp  = n - pi_n;      /* exponent for 2           */

    return pow2idx(exp);
}
