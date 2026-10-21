/*
 * A068318  Sum of prime factors of n-th semiprime.
 * https://oeis.org/A068318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the two prime factors of the n‑th semiprime
 * Range:     u8 n <= 156, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Global cache for generated semiprimes. */
static seq_t   *semiprimes = NULL;   /* array of nth semiprime (seq_t form) */
static size_t  semi_len   = 0;       /* number of stored semiprimes          */
static size_t  semi_cap   = 0;       /* current capacity of the array        */

/* Smallest prime factor sieve. */
static uint64_t *spf      = NULL;     /* spf[i] = smallest prime factor of i */
static size_t    spf_lim  = 0;        /* size of the spf table               */

/* ------------------------------------------------------------------ */
/* Allocate or enlarge the spf table up to `limit`.                    */
/* ------------------------------------------------------------------ */
static void ensure_spf(size_t limit)
{
    if (limit <= spf_lim) return;          /* already large enough */

    /* Allocate new table. */
    uint64_t *new_spf = (uint64_t*)realloc(spf, (limit + 1) * sizeof(uint64_t));
    if (!new_spf) return;                  /* unlikely in tests */
    spf     = new_spf;
    spf_lim = limit;

    /* Initialise with identity. */
    for (size_t i = 0; i <= limit; ++i) spf[i] = i;

    /* Sieve of Eratosthenes for smallest prime factor. */
    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == i) {                 /* i is prime */
            for (size_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Rebuild the list of semiprimes up to `limit`.                       */
/* ------------------------------------------------------------------ */
static void rebuild_semiprimes(size_t limit)
{
    /* Reset count. */
    semi_len = 0;

    /* Ensure capacity of the cache. */
    if (semi_cap < 1024) {
        free(semiprimes);
        semiprimes = (seq_t*)malloc(1024 * sizeof(seq_t));
        semi_cap   = 1024;
    }

    /* Iteratively find semiprimes. */
    for (size_t i = 4; i <= limit; ++i) {
        if (spf[i] != i) {                 /* composite */
            uint64_t p = spf[i];
            uint64_t q = i / p;
            if (q <= limit && spf[q] == q) {   /* both factors prime */
                /* Grow cache if needed. */
                if (semi_len == semi_cap) {
                    semi_cap *= 2;
                    semiprimes = (seq_t*)realloc(semiprimes,
                                                 semi_cap * sizeof(seq_t));
                }
                semiprimes[semi_len++] = (seq_t)i;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Ensure we have at least `n` semiprimes precomputed.                 */
/* ------------------------------------------------------------------ */
static void ensure_semiprimes(idx_t n)
{
    if ((idx_t)semi_len >= n) return;      /* already enough */

    /* Initial estimate for required sieve limit. */
    size_t limit = 5 * (size_t)n;
    if (limit < 10) limit = 10;

    /* Keep expanding the sieve until we have enough semiprimes. */
    while (true) {
        ensure_spf(limit);
        rebuild_semiprimes(limit);
        if ((idx_t)semi_len >= n) break;
        limit *= 2;                         /* increase range and repeat */
    }
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system.                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                    /* out of range */

    ensure_semiprimes(n);

    seq_t sem = semiprimes[n - 1];           /* 0‑based indexing */

    /* Compute sum of its two prime factors. */
    size_t idx = (size_t)sem;                /* safe cast for array index */
    uint64_t p  = spf[idx];
    uint64_t q  = idx / p;
    uint64_t sum = p + q;

    return (seq_t)sum;
}
