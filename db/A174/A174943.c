/*
 * A174943  a(n) = Sum_{k<=n} A007955(k) * A008683(k) = Sum_{k<=n} A007955(k)
 *          * mu(k), where A007955(m) = product of divisors of m.
 * https://oeis.org/A174943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k<=n} A007955(k) * mu(k) * where A007955(k) =
 *            product of divisors of k = k^{d(k)/2} (d(k) = number of
 *            divisors) * and mu(k) is the Möbius function. * * The
 *            implementation uses a linear sieve to precompute the smallest
 *            prime * factor and the Möbius values up to the requested index.
 *            Div...
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 43, i64 n <= 43, i128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* fast exponentiation in seq_t: base^exp  (exp>=0)                          */
static inline seq_t pow_seq_uint(uint32_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    seq_t b   = (seq_t)base;
    while (exp)
    {
        if (exp & 1ULL) res *= b;
        b *= b;
        exp >>= 1;
    }
    return res;
}

/* static storage for memoisation */
static idx_t       computed_max   = 0;
static seq_t      *a_cache        = NULL;   /* cumulative sums 0..computed_max */
static int8_t     *mu_cache       = NULL;   /* Möbius values     -1/0/1     */
static int        *spf_cache      = NULL;   /* smallest prime factor */

/* recompute all values up to n (inclusive); frees old storage      */
static void recompute_up_to(idx_t n)
{
    if (n <= computed_max) return;           /* nothing to do */

    /* free previous tables if they exist */
    free(a_cache);
    free(mu_cache);
    free(spf_cache);

    /* allocate new tables */
    a_cache  = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    mu_cache = (int8_t*)malloc((n + 1) * sizeof(int8_t));
    spf_cache= (int*)   malloc((n + 1) * sizeof(int));

    if (!a_cache || !mu_cache || !spf_cache) {
        /* allocation failed – abort to avoid undefined behaviour   */
        exit(1);
    }

    /* initialise spf_cache */
    for (idx_t i = 0; i <= n; ++i) spf_cache[i] = 0;

    /* linear sieve to fill spf_cache and mu_cache */
    int *primes = (int*)malloc((n + 1) * sizeof(int));   /* list of primes  */
    size_t prime_cnt = 0;

    mu_cache[1] = 1;
    for (idx_t i = 2; i <= n; ++i)
    {
        if (spf_cache[i] == 0)          /* i is prime               */
        {
            spf_cache[i] = (int)i;
            primes[prime_cnt++] = (int)i;
            mu_cache[i] = -1;
        }
        for (size_t j = 0; j < prime_cnt; ++j)
        {
            int p = primes[j];
            idx_t ip = i * (idx_t)p;
            if (ip > n) break;
            spf_cache[ip] = p;
            if (i % p == 0) {            /* p divides i              */
                mu_cache[ip] = 0;
                break;
            } else {
                mu_cache[ip] = (int8_t)(-mu_cache[i]);
            }
        }
    }
    free(primes);

    /* compute cumulative sums a_cache[0..n]                         */
    a_cache[0] = (seq_t)0;
    seq_t sum   = (seq_t)0;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t term = (seq_t)0;
        if (mu_cache[k] == 0) {
            term = (seq_t)0;
        } else {
            /* factorize k using spf_cache to obtain divisor count d(k)
               and the Möbius sign.                                   */
            idx_t tmp   = k;
            int  d_val  = 1;          /* number of divisors          */
            int  mu_val = 1;          /* -1 for each prime factor    */
            bool square_free = true;

            while (tmp > 1)
            {
                int p = spf_cache[tmp];
                int cnt = 0;
                while (tmp % p == 0) { tmp /= p; ++cnt; }
                if (cnt > 1) { square_free = false; break; }
                mu_val = -mu_val;
                d_val *= (cnt + 1);
            }

            if (square_free)
            {
                idx_t exp = (idx_t)(d_val / 2);   /* integer exponent   */
                term = pow_seq_uint((uint32_t)k, exp);
                if (mu_cache[k] < 0) term = -(term);
            }
        }
        sum += term;
        a_cache[k] = sum;
    }
    computed_max = n;
}

/* required external function                                                */
seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;          /* behaviour below offset irrelevant */
    if (n > computed_max)
        recompute_up_to(n);
    return a_cache[n];
}
