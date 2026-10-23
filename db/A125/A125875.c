/*
 * A125875  Odd numbers k such that cos(2*Pi/k) is an algebraic number of a
 *          13-smooth degree, but not 11-smooth.
 * https://oeis.org/A125875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd integer k for which (φ(k)/2) is a 13‑smooth
 *            number divisible by 13 ( i.e. φ(k)/2 has no prime factors
 *            greater than 13 but contains a factor 13). Sequence is OEIS
 *            A125875
 * Range:     u8 none, u16 none, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* forward declaration of seq_t and idx_t from build */
seq_t a(idx_t n);

/* static cache of found sequence values */
static seq_t *cache_vals      = NULL;   /* array of found k's          */
static size_t cache_len       = 0;      /* number of values stored     */
static size_t cache_cap       = 0;      /* allocated capacity          */
static uint32_t sieve_limit   = 0;      /* current upper bound to sieve*/
static bool      initialized  = false;  /* has memoization been done   */

/* free any existing cache */
static void free_cache(void)
{
    free(cache_vals);
    cache_vals = NULL;
    cache_len  = 0;
    cache_cap  = 0;
}

/* build explicit cache up to new limit */
static void build_cache(uint32_t limit)
{
    /* free old cache if any */
    free_cache();

    /* allocate temporary arrays for φ and composite markers */
    uint32_t *phi      = (uint32_t*)malloc((limit + 1) * sizeof(uint32_t));
    uint8_t  *is_comp  = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    if (!phi || !is_comp) {
        exit(1);   /* out of memory, abort */
    }

    /* initialise φ[i] = i and composite flags */
    for (uint32_t i = 0; i <= limit; ++i) {
        phi[i]    = i;
        is_comp[i]= 0;
    }

    /* linear sieve to compute φ for all numbers up to limit */
    for (uint32_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {                 /* p is prime */
            for (uint32_t j = p; j <= limit; j += p) {
                is_comp[j] = 1;
                phi[j] -= phi[j] / p;
            }
        }
    }

    /* buffer for 13‑smooth residues */
    const uint32_t primes13[6] = {2,3,5,7,11,13};

    /* allocate cache with some initial capacity */
    cache_cap = 1024;
    cache_vals = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache_vals) {
        free(phi); free(is_comp);
        exit(1);
    }

    /* scan odd k's and collect those fulfilling the condition */
    for (uint32_t k = 3; k <= limit; k += 2) {
        uint32_t phik = phi[k];
        if (phik & 1) continue;                         /* φ(k) must be even */
        seq_t val = (seq_t)(phik >> 1);                 /* φ(k)/2          */
        if ((uint32_t)(val % 13) != 0) continue;        /* must be divisible by 13 */

        uint32_t tmp = (uint32_t)val;
        for (size_t i = 0; i < 6; ++i) {
            uint32_t p = primes13[i];
            while (tmp % p == 0) tmp /= p;
        }
        if (tmp == 1) {                                 /* no large primes */
            if (cache_len == cache_cap) {               /* grow cache if needed */
                cache_cap <<= 1;
                cache_vals = (seq_t*)realloc(cache_vals,
                                              cache_cap * sizeof(seq_t));
                if (!cache_vals) { free(phi); free(is_comp); exit(1); }
            }
            cache_vals[cache_len++] = (seq_t)k;
        }
    }

    free(phi);
    free(is_comp);
}

/* ensure that cache contains at least n terms */
static void ensure_terms(idx_t n)
{
    size_t target = (size_t)n;
    if (initialized && cache_len >= target) return;

    /* start with a reasonable bound and grow if necessary */
    if (!sieve_limit) sieve_limit = 2000000;   /* start at 2M */

    while (1) {
        build_cache(sieve_limit);
        if (cache_len >= target) break;
        sieve_limit <<= 1;                    /* double sieving bound */
    }
    initialized = true;
}

/* public function required by the problem */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* undefined, but safe */
    ensure_terms(n);
    return cache_vals[(size_t)n - 1];
}
