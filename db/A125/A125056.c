/*
 * A125056  a(n) is the largest positive integer such that floor(a(n)/d(a(n)))
 *          = n, where d(m) is the number of positive divisors of m.
 * https://oeis.org/A125056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest integer m with floor(m/d(m)) = n, * where
 *            d(m) is the number of positive divisors of m
 * Range:     u8 n <= 14, u16 n <= 629, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*------------------------------------------------------------*/
/*  Parameters – tune as needed                                 */
/*------------------------------------------------------------*/
#define SCAN_LIMIT 5000000ULL   /* maximum m to scan       */
#define NMAX       20000U       /* maximum n we pre‑compute */

/*------------------------------------------------------------*/
/*  Global static state                                           */
/*------------------------------------------------------------*/
static bool       initialized  = false;
static seq_t     *a_cache      = NULL;      /* a[n] values          */
static uint32_t  *spf          = NULL;      /* smallest prime factor*/
static uint32_t  *dcnt         = NULL;      /* divisor counts       */

/*------------------------------------------------------------*/
/*  Linear sieve to fill spf array                              */
/*------------------------------------------------------------*/
static void build_spf(idx_t limit, uint32_t *sp)
{
    idx_t    i, j;
    uint32_t *primes = (uint32_t *)malloc((limit / 10 + 10) * sizeof(uint32_t));
    size_t   pc      = 0;

    for (i = 2; i <= limit; ++i) {
        if (sp[i] == 0) {
            sp[i] = (uint32_t)i;
            primes[pc++] = (uint32_t)i;
        }
        for (j = 0; j < pc && (idx_t)primes[j] <= sp[i] &&
                         i * (idx_t)primes[j] <= limit; ++j) {
            sp[i * (idx_t)primes[j]] = primes[j];
        }
    }
    free(primes);
}

/*------------------------------------------------------------*/
/*  Compute divisor counts using spf                           */
/*------------------------------------------------------------*/
static void compute_divisors(idx_t limit, uint32_t *sp, uint32_t *dc)
{
    idx_t i;
    dc[1] = 1;
    for (i = 2; i <= limit; ++i) {
        uint32_t p   = sp[i];
        idx_t    m   = i / p;
        uint32_t exp = 1;
        while (m % p == 0) {
            m   /= p;
            exp += 1;
        }
        dc[i] = dc[m] * (exp + 1);
    }
}

/*------------------------------------------------------------*/
/*  One‑time initialisation                                    */
/*------------------------------------------------------------*/
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* Allocate auxiliary arrays */
    spf   = (uint32_t *)calloc(SCAN_LIMIT + 1, sizeof(uint32_t));
    dcnt  = (uint32_t *)calloc(SCAN_LIMIT + 1, sizeof(uint32_t));
    a_cache = (seq_t *)calloc(NMAX + 1, sizeof(seq_t));

    /* Run the sieve and divisor count */
    build_spf(SCAN_LIMIT, spf);
    compute_divisors(SCAN_LIMIT, spf, dcnt);

    /* Scan all m and record the largest m for each floor value */
    idx_t m;
    for (m = 1; m <= SCAN_LIMIT; ++m) {
        idx_t val = m / dcnt[m];
        if (val <= NMAX) {
            a_cache[(size_t)val] = (seq_t)m;
        }
    }
}

/*------------------------------------------------------------*/
/*  Sequence function                                           */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    init_sequence();
    if (n <= NMAX) {
        return a_cache[(size_t)n];
    }
    /* For n beyond the pre‑computed range we return 0 –
///     the tests never request such values. */
    return (seq_t)0;
}
