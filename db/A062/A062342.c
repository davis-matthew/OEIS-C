/*
 * A062342  Primes whose sum of digits is a multiple of 8.
 * https://oeis.org/A062342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the n‑th prime whose sum of decimal digits is a multiple
 *            of 8. It is computed by a simple sieve of primes using trial
 *            division; each prime is tested for the digit‑sum condition. The
 *            result is memoised, so calls with arbitrary n are served in O(1)
 *            after the initial search
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* estimate a bound for realloc – used by ensure_cap */
static void ensure_cap(uint64_t **arr, size_t *len, size_t *cap, size_t need)
{
    if (*cap < need)
    {
        size_t newcap = *cap ? *cap : 16;
        while (newcap < need)
            newcap <<= 1;
        uint64_t *tmp = (uint64_t *)realloc(*arr, newcap * sizeof(**arr));
        if (!tmp)
            abort();
        *arr = tmp;
        *cap = newcap;
    }
}

/* sum of decimal digits of an uint64_t number */
static unsigned int digit_sum_u64(uint64_t x)
{
    unsigned int s = 0;
    while (x > 0)
    {
        s += (unsigned int)(x % 10);
        x /= 10;
    }
    return s;
}

seq_t a(idx_t n)
{
    /* terms are indexed from 1 */
    if (n < 1)
        return (seq_t)0;           /* undefined for n < 1 */

    /* static state for memoisation */
    static bool ready = false;
    static uint64_t *primes = NULL;            /* all primes found so far   */
    static size_t primes_len = 0, primes_cap = 0;
    static uint64_t *good   = NULL;            /* primes with digit‑sum %8 == 0 */
    static size_t good_len = 0, good_cap = 0;
    static uint64_t candidate = 3;             /* next odd to test   */

    if (!ready)
    {
        ready = true;
        /* initialise dynamic arrays */
        ensure_cap(&primes, &primes_len, &primes_cap, 0);
        ensure_cap(&good,   &good_len,   &good_cap,   0);
    }

    /* generate primes until we have enough good ones */
    while (good_len < (size_t)n)
    {
        uint64_t p = candidate;

        /* trivial check for 2 – digit sum 2, not useful, skip */
        bool is_prime = true;
        for (size_t i = 0; i < primes_len; ++i)
        {
            uint64_t q = primes[i];
            if (q * q > p)
                break;
            if (p % q == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            /* store prime */
            ensure_cap(&primes, &primes_len, &primes_cap, primes_len + 1);
            primes[primes_len++] = p;

            /* test digit‑sum condition */
            if (digit_sum_u64(p) % 8 == 0)
            {
                ensure_cap(&good, &good_len, &good_cap, good_len + 1);
                good[good_len++] = p;
            }
        }

        /* advance to next odd candidate */
        candidate += 2;
    }

    return (seq_t)good[n - 1];
}
