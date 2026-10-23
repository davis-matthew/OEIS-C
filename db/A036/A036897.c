/*
 * A036897  Square root of odd refactorable numbers.
 * https://oeis.org/A036897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sqrt(A036896(n)) (odd refactorable numbers are squares)
 * Range:     u8 n <= 29, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* sequence element type and index type are provided by the build.
   Do not typedef them here.                                  */

/* Smallest prime factor array and its current size (inclusive). */
static int *spf      = NULL;   /* spf[i] = smallest prime factor of i (i>=2) */
static size_t spf_sz = 0;      /* current allocated size of spf */

/* Cache of already computed a(n) terms and their state. */
static seq_t *cache   = NULL;     /* array of computed values */
static size_t  clen   = 0;        /* number of terms stored */
static size_t  cap    = 0;        /* allocated capacity of cache */

/* The next odd integer to test for refactorability. */
static idx_t next_odd = 1;

/* Flag indicating that initialization has been performed. */
static bool initialized = false;

/* -------------------------------------------------------------------- */
/* Build or rebuild the spf array up to at least 'limit' (inclusive).   */
/* The array is rebuilt completely; this is acceptable because it is   */
/* called only a few times.                                               */
static void build_spf(size_t limit)
{
    if (limit <= spf_sz) return;               /* already covers range */

    int *new_spf = (int *)malloc((limit + 1) * sizeof(int));
    for (size_t i = 0; i <= limit; ++i)
        new_spf[i] = (int)i;                   /* initialise */

    for (size_t i = 2; i * i <= limit; ++i) {
        if ((size_t)new_spf[i] == i) {        /* i is prime */
            for (size_t j = i * i; j <= limit; j += i)
                if ((size_t)new_spf[j] == j)  /* not yet set */
                    new_spf[j] = (int)i;
        }
    }

    new_spf[0] = new_spf[1] = 1;               /* fix unused entries */

    free(spf);
    spf = new_spf;
    spf_sz = limit;
}

/* -------------------------------------------------------------------- */
/* Ensure the cache can hold 'needed' elements.                          */
static void grow_cache(size_t needed)
{
    if (needed <= cap) return;

    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cap = new_cap;
}

/* -------------------------------------------------------------------- */
/* Compute a(n) : the n-th square root of an odd refactorable number.   */
seq_t a(idx_t n)
{
    if (!initialized) {                       /* one-time setup */
        build_spf(2000000);                   /* initial sieve size */
        initialized = true;
    }

    /* Generate terms until we have at least n of them.               */
    while (clen < (size_t)n) {
        /* Make sure spf covers the current odd number being tested.  */
        if ((size_t)next_odd > spf_sz - 1) {
            size_t new_limit = spf_sz * 2;
            build_spf(new_limit);
        }

        /* Factorise next_odd and compute τ = ∏(2e_i+1).                 */
        idx_t  s   = next_odd;
        seq_t  tau = 1;
        idx_t  temp = s;
        while (temp > 1) {
            int p = spf[(size_t)temp];
            seq_t exp = 0;
            do {
                temp /= p;
                ++exp;
            } while (temp % p == 0);
            tau *= ((seq_t)2 * exp + 1);
        }

        /* Check if s^2 is divisible by τ.                              */
        if (((unsigned long long)s * (unsigned long long)s) % tau == 0) {
            grow_cache(clen + 1);
            cache[clen++] = (seq_t)s;
        }

        next_odd += 2;   /* next odd number             */
    }

    return cache[(size_t)n - 1];
}
