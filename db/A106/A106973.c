/*
 * A106973  Primes of the form 5x^2-4xy+5y^2, with x and y nonnegative.
 * https://oeis.org/A106973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime of the form 5*x*x - 4*x*y + 5*y*y with x,y >=
 *            0
 * Range:     u8 n <= 3, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* One-time initialization -------------------------------------------- */
    static bool initialized = false;
    static const idx_t LIMIT = 4000000;          /* large enough for n≈10000   */
    static bool *is_prime = NULL;               /* sieve for primes          */
    static bool *represent = NULL;             /* marks numbers of the form  */
    static seq_t *terms = NULL;                /* array of primes of form   */
    static idx_t terms_cnt = 0;

    if (!initialized)
    {
        /* ---- Prime sieve ----------------------------------------------- */
        is_prime    = (bool*)malloc(sizeof(bool) * (LIMIT + 1));
        represent   = (bool*)malloc(sizeof(bool) * (LIMIT + 1));
        for (idx_t i = 0; i <= LIMIT; ++i)
        {
            is_prime[i]  = true;
            represent[i] = false;
        }
        is_prime[0] = is_prime[1] = false;
        for (idx_t p = 2; p * p <= LIMIT; ++p)
            if (is_prime[p])
                for (idx_t m = p * p; m <= LIMIT; m += p)
                    is_prime[m] = false;

        /* ---- Enumerate values of the quadratic form -------------------- */
        idx_t ymax = (idx_t)sqrt((long double)LIMIT / 5.0L);
        for (idx_t y = 0; y <= ymax; ++y)
        {
            idx_t maxDelta = 20 * LIMIT - 84 * y * y;
            if (maxDelta < 0) continue;
            idx_t sqrtY   = (idx_t)sqrt((long double)maxDelta);
            idx_t xmax = (4 * y + sqrtY) / 10;
            for (idx_t x = 0; x <= xmax; ++x)
            {
                idx_t val = 5 * x * x - 4 * x * y + 5 * y * y;
                if (is_prime[val])
                    represent[val] = true;
            }
        }

        /* ---- Collect the primes, in ascending order -------------------- */
        idx_t cap   = 1024;
        terms       = (seq_t*)malloc(sizeof(seq_t) * cap);
        terms_cnt   = 0;
        for (idx_t val = 5; val <= LIMIT; ++val)
        {
            if (is_prime[val] && represent[val])
            {
                if (terms_cnt == cap)         /* grow array if needed     */
                {
                    cap <<= 1;
                    terms = (seq_t*)realloc(terms, sizeof(seq_t) * cap);
                }
                terms[terms_cnt++] = (seq_t)val;
            }
        }
        initialized = true;
    }
    /* -------------------------------------------------------------------- */
    if (n == 0 || n > terms_cnt) return 0;
    return terms[n - 1];
}
