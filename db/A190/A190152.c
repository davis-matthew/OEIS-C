/*
 * A190152  Triangle of binomial coefficients binomial(3*n-k,3*n-3*k).
 * https://oeis.org/A190152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = binomial(3*r - k, 3*r - 3*k) where r =
 *            floor((sqrt(8*n+1)-1)/2), k = n - r*(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 39, u32 n <= 126, u64 n <= 419,
 *            u128 n <= 1565, big any n
 * Helpers:   build_primes_upto, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The values are produced on the rows of the triangle
   T(n,k)=binomial(3*n-k,3*n-3*k), indexed in row-major order. */

#include <math.h>
#include "../utils/pow_uint.h"       /* provides seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/seq_mul.h"        /* provides seq_t seq_mul(seq_t a, seq_t b); */
#include "../utils/build_primes_upto.h" /* provides void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include <stdlib.h> /* for malloc, free */

/* Prime factor exponent in n! (Legendre's formula) */
static int legendre_exp(idx_t n, idx_t p)
{
    int exp = 0;
    while (n)
    {
        n /= p;
        exp += (int)n;
    }
    return exp;
}

/* Cached prime list for efficient binomial calculation */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Ensure primes up to 'limit' are generated and cached */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (prime_list)
    {
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
    }
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Computes binomial(n,k) exactly using prime factorisation.
   Works for arbitrary-precision seq_t as well as builtin types. */
static seq_t binom_safe(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    if (k == 0 || k == n)
        return 1;

    ensure_primes(n);

    seq_t result = 1;

    for (size_t i = 0; i < prime_count && prime_list[i] <= n; ++i)
    {
        idx_t p = prime_list[i];
        int e = legendre_exp(n, p) - legendre_exp(k, p) - legendre_exp(n - k, p);
        if (e <= 0)
            continue;
        seq_t term = pow_uint((uint64_t)p, (idx_t)e);
        result = seq_mul(result, term);
    }

    return result;
}

seq_t a(idx_t n)
{
    /* Determine row r using the inverse of the triangular numbers:
       T(r) = r*(r+1)/2 <= n < T(r+1) */
    idx_t r = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);

    /* Correct possible rounding error from the floating‑point calculation */
    idx_t start = r * (r + 1) / 2;
    if (start > n)
    {
        while (start > n)
        {
            --r;
            start = r * (r + 1) / 2;
        }
    }
    else
    {
        while ((r + 1) * (r + 2) / 2 <= n)
        {
            ++r;
            start = r * (r + 1) / 2;
        }
    }

    /* Column index within the row (0‑based) */
    idx_t k = n - start;          /* 0 <= k <= r */

    /* Arguments of the binomial coefficient: T(n,k) = binom(3n-k, 3n-3k) */
    idx_t top  = 3 * r - k;
    idx_t bot  = 3 * r - 3 * k;

    return binom_safe(top, bot);
}
