/*
 * A172436  Smallest m such that the Moebius function takes successively, from
 *          m, n values 1,0,1,0,... ending with 1 or 0.
 * https://oeis.org/A172436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A172436:
   a(n) = the smallest m such that μ(m) = 1,
          μ(m+1) = 0,
          μ(m+2) = 1,
          μ(m+3) = 0,
          … alternately 1,0 for n values.
   For n > 16 such an m does not exist, and a(n) is defined as 0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mobius.h"

#define MAX_N 30000000ULL /* sufficient for all known values up to a(16) */

static seq_t cache[17] = {0};           /* memoised a(n), 1‑based */
static size_t cached_cnt = 0;           /* how many a(n) we have found */
static idx_t curr_m = 1;                /* current m being inspected */

static int8_t *mu = NULL;               /* Möbius values, -1,0,1 */
static uint8_t *composite = NULL;       /* temporary composite flag array */
static int *primes = NULL;              /* list of primes for linear sieve */
static size_t primes_cap = 0;
static size_t primes_cnt = 0;

/* ----------------------------------------------------------------------
   One‑time initialization of the μ‑table up to MAX_N.
   Uses the linear sieve algorithm; mu[1] = 1.
   ---------------------------------------------------------------------- */
static void init_mu(void)
{
    mu = (int8_t *)malloc((MAX_N + 1) * sizeof(int8_t));
    composite = (uint8_t *)malloc((MAX_N + 1) * sizeof(uint8_t));
    if (!mu || !composite)
        abort();
    primes_cap = MAX_N / 10 + 10;          /* enough for ~3.4 million primes */
    primes = (int *)malloc(primes_cap * sizeof(int));
    if (!primes)
        abort();

    mu[1] = 1;
    for (idx_t i = 2; i <= MAX_N; ++i)
    {
        if (!composite[i])
        {
            if (primes_cnt == primes_cap)
            {
                primes_cap *= 2;
                int *tmp = (int *)realloc(primes, primes_cap * sizeof(int));
                if (!tmp)
                    abort();
                primes = tmp;
            }
            primes[primes_cnt++] = (int)i;
            mu[i] = -1;
        }
        for (size_t pi = 0; pi < primes_cnt; ++pi)
        {
            idx_t p = (idx_t)primes[pi];
            idx_t ip = i * p;
            if (ip > MAX_N)
                break;
            composite[ip] = 1;
            if (i % p == 0)
            {
                mu[ip] = 0;
                break;
            }
            else
            {
                mu[ip] = -mu[i];
            }
        }
    }
    free(composite); /* no longer needed after sieve */
}

/* ----------------------------------------------------------------------
   Returns μ(x) as an int8_t (-1,0,1).  x is guaranteed to be ≤ MAX_N.
   ---------------------------------------------------------------------- */
static int8_t get_mu(idx_t x) { return mu[x]; }

/* ----------------------------------------------------------------------
   Core routine: compute a(n) with caching and incremental scanning.
   ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n > 16ULL)          /* problem statement: no terms for n >= 17 */
        return 0;

    if (!mu)                /* lazy initialisation */
        init_mu();

    if (cache[n])
        return cache[n];    /* already computed */

    /* Continue scanning until all 16 values are found or until we reach the
       required n.  The outer loop stops once all are cached to avoid
       repeated full scans. */
    while (cached_cnt < 16)
    {
        /* Test current m against all still unknown a(k) (k ≤ 16). */
        for (idx_t k = 1; k <= 16; ++k)
        {
            if (cache[k])
                continue;   /* already known */

            bool ok = true;
            /* pattern: μ(m + offset) == 1 for even offsets, 0 for odd offsets. */
            for (idx_t offset = 0; offset < k; ++offset)
            {
                int8_t mu_val = get_mu(curr_m + offset);
                if ((offset & 1) == 0)
                {
                    if (mu_val != 1)
                    {
                        ok = false;
                        break;
                    }
                }
                else
                {
                    if (mu_val != 0)
                    {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok)
            {
                cache[k] = curr_m;
                ++cached_cnt;
            }
        }
        if (cached_cnt == 16)   /* all terms known */
            break;
        ++curr_m;
    }

    return cache[n];
}
