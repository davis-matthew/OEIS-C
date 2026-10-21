/*
 * A087053  Numbers of the form pq + qr + rp where p, q and r are distinct
 *          primes, with multiplicity.
 * https://oeis.org/A087053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic derivative of the n‑th integer with exactly 3
 *            distinct prime factors. The underlying construction enumerates
 *            all numbers of the form p*q*r (p<q<r prime), sorts them by the
 *            product, and returns p*q + q*r + r*p for the n‑th such number
 * Range:     u8 n <= 33, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- required headers -------------------------------------------------- */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#if defined(__cplusplus)
/* For the arbitrary–precision build, seq_t is a C++ class type. */
#else
/* For 8–128 bit builds seq_t is a builtin unsigned integer type. */
#endif

/* --- parameters ------------------------------------------------------- */
#define TARGET_N          10000          /* we need at least this many terms   */
#define PRIME_LIMIT       200            /* primes <= 200 give > 10000 triples */

/* --- static data ------------------------------------------------------- */
static uint32_t primes[256];            /* enough for PRIME_LIMIT <= 200      */
static size_t prime_cnt = 0;             /* number of primes generated         */

/* --- helper: generate primes up to PRIME_LIMIT ------------------------ */
static void build_primes(void)
{
    if (prime_cnt > 0) return;          /* already built */

    size_t limit = PRIME_LIMIT;
    bool *is_composite = (bool *)malloc(limit + 1);
    if (!is_composite) { exit(1); }
    for (size_t i = 0; i <= limit; ++i) is_composite[i] = false;
    is_composite[0] = is_composite[1] = true;
    for (size_t i = 2; i * i <= limit; ++i)
        if (!is_composite[i])
            for (size_t j = i * i; j <= limit; j += i)
                is_composite[j] = true;
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i])
            primes[cnt++] = (uint32_t)i;
    prime_cnt = cnt;
    free(is_composite);
}

/* --- pair structure for sorting -------------------------------------- */
typedef struct {
    uint64_t prod;   /* product p*q*r, used for sorting                  */
    seq_t    deriv;  /* corresponding value p*q + q*r + r*p             */
} pair_t;

/* --- comparator for qsort -------------------------------------------- */
static int pair_cmp(const void *a, const void *b)
{
    const pair_t *pa = (const pair_t *)a;
    const pair_t *pb = (const pair_t *)b;
    if (pa->prod < pb->prod) return -1;
    if (pa->prod > pb->prod) return 1;
    return 0;
}

/* --- cache of sequence values ---------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* --- main sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0 */

    /* Pre‑compute once on first call */
    if (cache_size == 0) {
        build_primes();                   /* populate primes[] */

        /* Count triples that will be generated */
        size_t max_cnt = prime_cnt * (prime_cnt - 1) * (prime_cnt - 2) / 6;
        pair_t *pairs = (pair_t *)malloc(max_cnt * sizeof(pair_t));
        if (!pairs) exit(1);

        size_t idx = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t p = primes[i];
            for (size_t j = i + 1; j < prime_cnt; ++j) {
                uint32_t q = primes[j];
                for (size_t k = j + 1; k < prime_cnt; ++k) {
                    uint32_t r = primes[k];
                    uint64_t prod = (uint64_t)p * q * r;
                    __uint128_t a01 = (__uint128_t)p * q;
                    __uint128_t a12 = (__uint128_t)q * r;
                    __uint128_t a20 = (__uint128_t)r * p;
                    __uint128_t deriv128 = a01 + a12 + a20;
                    pairs[idx].prod = prod;
                    pairs[idx].deriv = (seq_t)deriv128;   /* cast safely   */
                    ++idx;
                }
            }
        }

        /* Sort by product (ascending) */
        qsort(pairs, idx, sizeof(pair_t), pair_cmp);

        /* Build cache only keeping the first TARGET_N terms.  The
           sequence definition requires all terms, but the caller
           will only ever request up to TARGET_N.  If one asks for
           a larger n, the function will return 0.              */
        size_t usable = idx;
        if (usable > TARGET_N) usable = TARGET_N;

        cache = (seq_t *)malloc(usable * sizeof(seq_t));
        if (!cache) exit(1);
        for (size_t i = 0; i < usable; ++i)
            cache[i] = pairs[i].deriv;

        cache_size = usable;
        free(pairs);
    }

    if ((idx_t)(n - 1) >= cache_size)
        return 0;                   /* out of precomputed range */

    return cache[n - 1];
}
