/*
 * A154365  Numbers n such that the smallest prime factor of composite(n) +
 *          the largest prime factor of composite(n) is prime and the sum of
 *          prime factors (with multiplicity) of composite(n) is prime.
 * https://oeis.org/A154365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154365: Indices n such that composite(n) satisfies
   (smallest prime factor + largest prime factor) is prime and
   the sum of prime factors (with multiplicity) is prime.
   This code searches for the required indices sequentially. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* ------------------------------------------------------------ */
/* Static state for the sieve, spf table, and composite list. */
/* ------------------------------------------------------------ */
static bool     initialized = false;
static idx_t  *spf      = NULL;   /* smallest prime factor for each n */
static idx_t  *comps    = NULL;   /* list of composite numbers */
static size_t   comp_len = 0;      /* number of composites stored */
static size_t   comp_cap = 0;      /* allocated capacity for comps */

/* Indices (1‑based) of composites that satisfy the property. */
static idx_t  *idxs     = NULL;
static size_t   idx_len = 0;       /* number of satisfying indices found */
static size_t   idx_cap = 0;       /* allocated capacity for idxs */

/* The last composite index that was examined. */
static idx_t   last_tested = 0;

/* ------------------------------------------------------------ */
/* Helper to initialise the sieve and composite list.  The limit
   is chosen large enough for the expected workload.              */
/* ------------------------------------------------------------ */
static void init_data(void)
{
    if (initialized) return;
    initialized = true;

    /* -------------------------------------------------------- */
    /* 1. Define an upper bound for the sieve.                 */
    /* -------------------------------------------------------- */
    const idx_t SIEVE_LIMIT = 5000000ULL;   /* 5 million is ample for n≤10000 */

    /* -------------------------------------------------------- */
    /* 2. Allocate and build the spf (smallest prime factor)   */
    /*    array.                                               */
    /* -------------------------------------------------------- */
    spf = (idx_t *)malloc((SIEVE_LIMIT + 1) * sizeof(idx_t));
    if (!spf) exit(EXIT_FAILURE);

    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            if (i > SIEVE_LIMIT / i) continue;  /* avoid overflow of i*i */
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* -------------------------------------------------------- */
    /* 3. Gather all composite numbers up to SIEVE_LIMIT.      */
    /* -------------------------------------------------------- */
    comp_cap = SIEVE_LIMIT;          /* upper bound, will shrink after */
    comps    = (idx_t *)malloc(comp_cap * sizeof(idx_t));
    if (!comps) exit(EXIT_FAILURE);

    for (idx_t i = 4; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] != i) {                         /* i is composite */
            if (comp_len >= comp_cap) {            /* grow storage if needed */
                comp_cap <<= 1;                    /* double capacity */
                idx_t *tmp = (idx_t *)realloc(comps, comp_cap * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                comps = tmp;
            }
            comps[comp_len++] = i;
        }
    }

    /* -------------------------------------------------------- */
    /* 4. Prepare the array that will hold satisfying indices. */
    /* -------------------------------------------------------- */
    idx_cap = 1024;
    idxs    = (idx_t *)malloc(idx_cap * sizeof(idx_t));
    if (!idxs) exit(EXIT_FAILURE);
}

/* ------------------------------------------------------------ */
/* Test whether the composite number at 1‑based index `idx`
   satisfies the required property.                           */
/* ------------------------------------------------------------ */
static bool good_index(idx_t idx)
{
    if (idx == 0 || idx > comp_len) return false;

    idx_t m = comps[idx - 1];                 /* the composite number */

    /* Smallest prime factor */
    idx_t spf_m = spf[m];

    /* Largest prime factor and sum of prime factors (with multiplicity) */
    idx_t largest = spf_m;
    idx_t sum_pf  = 0;
    idx_t tmp     = m;
    while (tmp > 1) {
        idx_t p = spf[tmp];
        if (p > largest) largest = p;
        while (tmp % p == 0) {
            sum_pf += p;
            tmp   /= p;
        }
    }

    /* Two conditions – use is_prime_big for arbitrary size. */
    if (!is_prime_big((seq_t)(spf_m + largest))) return false;
    if (!is_prime_big((seq_t)sum_pf))              return false;
    return true;
}

/* ------------------------------------------------------------ */
/* Ensure that at least `need` satisfying indices have been
   found and stored in `idxs`.                                 */
/* ------------------------------------------------------------ */
static void ensure_terms(size_t need)
{
    init_data();

    while (idx_len < need) {
        ++last_tested;
        if (last_tested > comp_len) {
            /* In practice this should never happen with the chosen
               sieve limit for the required range.               */
            exit(EXIT_FAILURE);
        }
        if (good_index(last_tested)) {
            if (idx_len >= idx_cap) {
                idx_cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(idxs, idx_cap * sizeof(idx_t));
                if (!tmp) exit(EXIT_FAILURE);
                idxs = tmp;
            }
            idxs[idx_len++] = last_tested;
        }
    }
}

/* ------------------------------------------------------------ */
/* Public function required by the OEIS build.  It returns the
   n‑th element of A154365.  For n < 1 we return 0.          */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_terms((size_t)n);
    return (seq_t)idxs[(size_t)(n - 1)];
}
