/*
 * A072460  Smallest even nontotient number whose largest prime divisor equals
 *          the n-th prime.
 * https://oeis.org/A072460
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 911, u32 n <= 9904, u64 n <= 9904,
 *            u128 n <= 9904, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the smallest even nontotient number whose largest prime divisor equals the n‑th prime
 * (OEIS A072460).  The computation uses a one‑time sieve that
 * pre‑computes all totient values up to a fixed limit and all primes up to that limit.
 * For each n (n ≥ 3) we enumerate candidates B = 2·p·k (p = prime(n), k ≥ 1)
 * with no prime factor larger than p and return the first B that is not a totient.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint8_t, uint32_t, uint64_t, idx_t, seq_t */
#include <stdlib.h>   /* malloc, calloc, free, abort */
#include <stdbool.h>  /* bool, true, false */

#include "../utils/build_primes.h"

/* --------------------------------------------------------------------- */
/*  Static data, initialised once on first call of a()                     */
/* --------------------------------------------------------------------- */
static idx_t *g_primes = NULL;          /* array of primes up to LIMIT */
static size_t g_prime_cnt = 0;          /* number of primes in g_primes    */

static unsigned int g_totient_limit;              /* maximum index in totient tables  */
static unsigned char *g_is_totient = NULL;        /* boolean table: 1 if value is a totient */

/* forward declarations ------------------------------------------------- */
static void init_sequence(void);
static inline idx_t nth_prime(size_t k);          /* 1‑based index */
static inline bool   k_has_factor_gt_p(uint64_t k, uint64_t p);
static inline bool   is_totient_uint(unsigned int v);

/* --------------------------------------------------------------------- */
/*  Initialise the global tables on first use                            */
/* --------------------------------------------------------------------- */
static void init_sequence(void)
{
    if (g_primes != NULL) return;          /* already initialised */

    /* ------------------------------------------------------------------ */
    /*  Choose a sieve limit large enough for n ≤ 10000.                  */
    /*  The 10 million limit comfortably covers a(n) for these indices.   */
    /* ------------------------------------------------------------------ */
    const unsigned int LIMIT = 20000000;    /* 20·10⁶ – can be reduced if needed */
    g_totient_limit = LIMIT;

    /* ---- primes ------------------------------------------------------- */
    build_primes(LIMIT, &g_primes, &g_prime_cnt);

    /* ---- phi array (uint32_t) --------------------------------------- */
    uint32_t *phi = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!phi) abort();    /* unlikely, but safe */
    for (unsigned int i = 0; i <= LIMIT; ++i) phi[i] = i;   /* initialise */

    /* sieve φ by multiplying over primes */
    for (size_t pi = 0; pi < g_prime_cnt; ++pi) {
        unsigned int p = (unsigned int)g_primes[pi];
        for (unsigned int j = p; j <= LIMIT; j += p) {
            phi[j] = phi[j] / p * (p - 1);
        }
    }

    /* ---- totient flag table ------------------------------------------- */
    g_is_totient = (unsigned char *)calloc(LIMIT + 1, sizeof(unsigned char));
    if (!g_is_totient) abort();

    for (unsigned int i = 2; i <= LIMIT; ++i) {
        unsigned int val = phi[i];
        if (val <= LIMIT) g_is_totient[val] = 1;
    }

    /* free memory that is no longer needed */
    free(phi);
}

/* --------------------------------------------------------------------- */
/*  Helper: return the k‑th prime (1‑based).  k must be < g_prime_cnt   */
/* --------------------------------------------------------------------- */
static inline idx_t nth_prime(size_t k)
{
    return g_primes[k - 1];
}

/* --------------------------------------------------------------------- */
/*  Check whether k contains any prime factor larger than p              */
/* --------------------------------------------------------------------- */
static inline bool k_has_factor_gt_p(uint64_t k, uint64_t p)
{
    if (k <= 1) return false;

    uint64_t tmp = k;
    for (size_t i = 0; i < g_prime_cnt; ++i) {
        uint64_t q = g_primes[i];
        if (q * q > tmp) break;
        if (tmp % q == 0) {
            if (q > p) return true;
            while (tmp % q == 0) tmp /= q;
            if (tmp == 1) break;
        }
    }
    /* remaining factor, if any, is prime > sqrt(tmp) */
    return tmp > 1 && tmp > p;
}

/* --------------------------------------------------------------------- */
/*  Return true iff v <= limit and v is a totient value                  */
/* --------------------------------------------------------------------- */
static inline bool is_totient_uint(unsigned int v)
{
    return v <= g_totient_limit ? g_is_totient[v] : false;
}

/* --------------------------------------------------------------------- */
/*  Main sequence routine                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined for indices < 3 */

    init_sequence();              /* one‑time initialisation */

    uint64_t p = nth_prime((size_t)n);   /* n‑th prime (1‑based) */
    uint64_t k = 1;

    while (1) {                   /* loop over k = 1,2,3,… */
        if (!k_has_factor_gt_p(k, p)) {
            uint64_t m = 2 * p * k;
            if (m <= (uint64_t)g_totient_limit && !is_totient_uint((unsigned int)m))
                return (seq_t)m;
        }
        ++k;
    }
}
