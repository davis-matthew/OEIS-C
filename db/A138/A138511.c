/*
 * A138511  Semiprimes where the larger prime factor is greater than the
 *          square of the smaller prime factor, short: semiprimes p*q, p^2 <
 *          q.
 * https://oeis.org/A138511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   A138511 = semiprimes p*q with p < q and p^2 < q (sorted
 *            ascending)
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Build a list of primes up to 'limit' (inclusive).  The result
   is allocated and must be freed by the caller. */
static void build_primes(uint64_t limit,
                         uint64_t **primes_out,
                         size_t *cnt_out)
{
    /* Sieve of Eratosthenes using a bitmap of bytes */
    unsigned char *is_composite = (unsigned char*)calloc(limit + 1, sizeof(unsigned char));
    if (!is_composite) {          /* allocation failure – very unlikely */
        *primes_out = NULL;
        *cnt_out   = 0;
        return;
    }

    size_t i, j;
    for (i = 2; i * i <= limit; ++i) {
        if (!is_composite[i]) {
            for (j = i * i; j <= limit; j += i)
                is_composite[j] = 1;
        }
    }

    /* Allocate space for primes.  Rough estimate: limit / 10 is more than enough
       for the range we need. */
    size_t primes_cap = limit / 10 + 10;
    uint64_t *primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) {
        free(is_composite);
        *primes_out = NULL;
        *cnt_out   = 0;
        return;
    }

    size_t primes_cnt = 0;
    for (i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            if (primes_cnt == primes_cap) {   /* grow if necessary */
                primes_cap *= 2;
                uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
                if (!tmp) { /* allocation failed */
                    free(primes);
                    free(is_composite);
                    *primes_out = NULL;
                    *cnt_out   = 0;
                    return;
                }
                primes = tmp;
            }
            primes[primes_cnt++] = i;
        }
    }

    free(is_composite);
    *primes_out = primes;
    *cnt_out    = primes_cnt;
}

/* Comparator for qsort on an array of seq_t values */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t *x = (const seq_t*)a;
    const seq_t *y = (const seq_t*)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* Global cache for the precomputed sequence */
static seq_t *g_seq       = NULL;
static size_t g_seq_len   = 0;

/* Build the sequence up to a pre-defined product limit that guarantees
   at least 10 000 terms.  The cache is built lazily on the first call to
   a().  Subsequent calls reuse the cache. */
static void build_sequence(void)
{
    if (g_seq != NULL) return;  /* already built */

    /* Choose a product limit that comfortably yields more than 10 000 terms.
       Empirically 20 000 000 is sufficient. */
    const uint64_t LIMIT_PRODUCT = 20000000ULL;

    /* Generate primes up to LIMIT_PRODUCT (p will never exceed sqrt(LIMIT_PRODUCT)
       and q will at most be LIMIT_PRODUCT).  This is the most
       expensive step but is done only once. */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes(LIMIT_PRODUCT, &primes, &primes_cnt);
    if (!primes) return;                  /* failure – should not happen */

    /* Estimate how many semiprimes we might generate and preallocate a
       reasonable buffer.  For 20 M the number of qualifying semiprimes is
       well below 200 000. */
    size_t semis_cap = 200000;
    seq_t *semis = (seq_t*)malloc(semis_cap * sizeof(seq_t));
    if (!semis) { free(primes); return; }

    size_t semis_len = 0;
    /* Helper macro to grow the array when needed */
    #define GROW_SEMIS()                                         \
        do {                                                     \
            semis_cap *= 2;                                      \
            seq_t *tmp = (seq_t*)realloc(semis, semis_cap * sizeof(seq_t)); \
            if (!tmp) { free(primes); free(semis); return; }     \
            semis = tmp;                                         \
        } while (0)

    /* Generate qualifying semiprimes */
    for (size_t ip = 0; ip < primes_cnt; ++ip) {
        uint64_t p = primes[ip];
        /* If p^2 already exceeds LIMIT_PRODUCT, no q can produce a product
           <= LIMIT_PRODUCT.  We stop the outer loop. */
        if ((seq_t)p * (seq_t)p > LIMIT_PRODUCT)
            break;

        /* Find the first prime q > p^2 */
        uint64_t sq = p * p;
        size_t jq = ip + 1;          /* ensure p < q */
        while (jq < primes_cnt && primes[jq] <= sq) ++jq;

        /* Iterate over q and generate products <= LIMIT_PRODUCT */
        for (size_t iq = jq; iq < primes_cnt; ++iq) {
            uint64_t q = primes[iq];
            seq_t prod = (seq_t)p * (seq_t)q;
            if (prod > LIMIT_PRODUCT) break;   /* further q will only increase prod */

            if (semis_len == semis_cap) GROW_SEMIS();
            semis[semis_len++] = prod;
        }
    }

    free(primes);

    /* Sort the array so that it is in ascending order */
    qsort(semis, semis_len, sizeof(seq_t), cmp_seq);

    /* Cache the result */
    g_seq     = semis;
    g_seq_len = semis_len;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* undefined for n < 1, safe fallback */
    build_sequence();                 /* lazy initialization */
    if ((idx_t)n > g_seq_len) return 0;/* out of precomputed range */
    return g_seq[n - 1];
}
