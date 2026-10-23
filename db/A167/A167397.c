/*
 * A167397  n-th single (or isolated or non-twin) prime minus n.
 * https://oeis.org/A167397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007510(n) - A000027(n) (n ≥ 1)
 * Range:     u8 n <= 24, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th non‑twin (single) prime and subtracts n.
   The function works for any build of seq_t and idx_t. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* --- static state for prime tables ----------------------------------- */
static uint8_t  *is_comp   = NULL;   /* composite flag (0 = prime candidate) */
static size_t    comp_size = 0;      /* current size of is_comp (incl. 0) */

static uint32_t *primes    = NULL;   /* list of all primes up to current limit */
static size_t    primes_cnt  = 0;
static size_t    primes_cap  = 0;

static uint32_t *singles   = NULL;   /* list of non‑twin primes */
static size_t    singles_cnt = 0;
static size_t    singles_cap = 0;

/* --- helper: estimate upper bound for the n‑th single prime ------------
   Uses the prime number theorem with a safety factor.  */
static inline idx_t estimate_limit(idx_t n)
{
    if (n < 6) return (idx_t)100;
    double dn   = (double)n;
    double est  = dn * (log(dn) + log(log(dn)));
    est        *= 1.5;               /* safety factor for single primes */
    return (idx_t)est + 100;
}

/* --- rebuild all tables up to new_limit (inclusive) ------------------- */
static void rebuild_tables(idx_t new_limit)
{
    /* Allocate or resize the is_comp array. */
    size_t new_size = (size_t)new_limit + 1;
    uint8_t *tmp = (uint8_t *)realloc(is_comp, new_size * sizeof(uint8_t));
    if (!tmp) exit(EXIT_FAILURE);
    is_comp = tmp;
    comp_size = new_size;
    for (size_t i = 0; i < new_size; ++i) is_comp[i] = 0;

    /* Eratosthenes sieve. */
    size_t sqrt_lim = (size_t)sqrt((double)new_limit);
    for (size_t i = 2; i <= sqrt_lim; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= new_limit; j += i) {
                is_comp[j] = 1;
            }
        }
    }

    /* Collect primes. */
    if (primes_cap < 1024) {
        primes_cap = 1024;
        uint32_t *p = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
        if (!p) exit(EXIT_FAILURE);
        primes = p;
    }
    primes_cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt >= primes_cap) {
                primes_cap <<= 1;
                uint32_t *p = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!p) exit(EXIT_FAILURE);
                primes = p;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }

    /* Collect non‑twin primes (singletons). */
    if (singles_cap < 1024) {
        singles_cap = 1024;
        uint32_t *s = (uint32_t *)realloc(singles, singles_cap * sizeof(uint32_t));
        if (!s) exit(EXIT_FAILURE);
        singles = s;
    }
    singles_cnt = 0;
    for (size_t k = 0; k < primes_cnt; ++k) {
        uint32_t p = primes[k];
        if (p == 2) {                      /* 2 is always a singleton */
            if (singles_cnt >= singles_cap) {
                singles_cap <<= 1;
                uint32_t *s = (uint32_t *)realloc(singles, singles_cap * sizeof(uint32_t));
                if (!s) exit(EXIT_FAILURE);
                singles = s;
            }
            singles[singles_cnt++] = p;
            continue;
        }
        int twin = 0;
        if (p >= 3 && !is_comp[p - 2]) twin = 1;             /* p-2 is prime */
        if (!twin && p + 2 <= new_limit && !is_comp[p + 2]) twin = 1; /* p+2 is prime */
        if (!twin) {                                         /* single prime */
            if (singles_cnt >= singles_cap) {
                singles_cap <<= 1;
                uint32_t *s = (uint32_t *)realloc(singles, singles_cap * sizeof(uint32_t));
                if (!s) exit(EXIT_FAILURE);
                singles = s;
            }
            singles[singles_cnt++] = p;
        }
    }
}

/* --- ensure we have at least n singletons ready ----------------------- */
static void ensure_singles(size_t n)
{
    if (n <= singles_cnt) return;          /* already enough */
    idx_t limit = estimate_limit((idx_t)n);
    rebuild_tables(limit);
    while (singles_cnt < n) {
        limit *= 2;                         /* extend limit */
        rebuild_tables(limit);
    }
}

/* --- sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* behaviour below offset is irrelevant */
    ensure_singles((size_t)n);
    uint32_t prime_single = singles[n - 1]; /* primes stored 0‑based */
    return (seq_t)prime_single - (seq_t)n;
}
