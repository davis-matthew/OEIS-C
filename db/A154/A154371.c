/*
 * A154371  Duplicate of A119613.
 * https://oeis.org/A154371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n such that * n is composite, and * (largest prime factor
 *            of n) - (smallest prime factor of n) is prime
 * Range:     i8 n <= 34, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation builds a sieve once the first time the function is
 * called, collects all qualifying numbers up to a fixed limit, and
 * returns the n‑th element from the cached list.  The limit is chosen
 * large enough to contain the first 10 000 terms of the sequence. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/*--- Configuration ----------------------------------------------------*/
#define SIEVE_LIMIT 1000000          /* upper bound for sieve          */
#define CACHE_MAX   200000           /* size of static candidate cache */

/*--- Static tables used by the algorithm ------------------------------*/
static uint64_t spf[SIEVE_LIMIT + 1];   /* smallest prime factor       */
static uint64_t lpf[SIEVE_LIMIT + 1];   /* largest prime factor        */
static uint8_t  is_prime_flag[SIEVE_LIMIT + 1]; /* prime marker          */
static seq_t    cache[CACHE_MAX];      /* list of qualifying numbers  */
static size_t   cache_len = 0;

/*--- One‑time initialization routine ---------------------------------*/
static void init_sieve(void)
{
    /* initialise arrays */
    for (size_t i = 0; i <= SIEVE_LIMIT; ++i)
    {
        spf[i]          = 0;
        lpf[i]          = 0;
        is_prime_flag[i] = 0;
    }

    /* sieve for primes, smallest and largest prime factors */
    for (uint64_t p = 2; p <= SIEVE_LIMIT; ++p)
    {
        if (spf[p] == 0)           /* p is prime */
        {
            spf[p] = p;
            lpf[p] = p;
            is_prime_flag[p] = 1;

            for (uint64_t j = p; j <= SIEVE_LIMIT; j += p)
            {
                if (spf[j] == 0)
                    spf[j] = p;    /* first hit = smallest prime */
                lpf[j] = p;        /* last hit = largest prime     */
            }
        }
    }

    /* collect qualifying numbers into cache */
    for (uint64_t n = 4; n <= SIEVE_LIMIT && cache_len < CACHE_MAX; ++n)
    {
        if (spf[n] == 0)           /* prime, skip */
            continue;

        uint64_t minp = spf[n];
        uint64_t maxp = lpf[n];

        if (minp == maxp)          /* prime power, no distinct primes */
            continue;

        uint64_t diff = maxp - minp;
        if (diff >= 2 && is_prime_flag[diff])
        {
            cache[cache_len++] = (seq_t)n;
        }
    }
}

/*--- Public sequence function ----------------------------------------*/
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized)
    {
        init_sieve();
        initialized = true;
    }

    if (n == 0 || n > cache_len)   /* undefined for n outside the cache */
        return 0;

    /* sequence indices in OEIS start at 1 */
    return cache[n - 1];
}
