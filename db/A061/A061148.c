/*
 * A061148  Smallest positive integer for which the number of divisors is a
 *          product of 2 distinct primes: Min{x; d[x]=pq}.
 * https://oeis.org/A061148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A005179(A006881(n))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   pow2, pow3, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*      OEIS A061148: Smallest positive integer having a divisor
 *      count equal to the nth product of two distinct primes. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"
#include "../utils/seq_mul.h"

/* -------------------------------------------------------------------------- */
/*  Data structures and static tables                                         */
/* -------------------------------------------------------------------------- */
typedef struct {
    uint64_t prod;      /* product of the two primes */
    uint32_t p;         /* smaller prime  (p < q)  */
    uint32_t q;         /* larger prime   */
} semip_t;

static semip_t *semis      = NULL;  /* sorted list of semiprimes   */
static size_t   semi_cnt   = 0;      /* number of entries in semis   */
static uint32_t *primes    = NULL;  /* list of primes up to LIMIT   */
static size_t   prime_cnt  = 0;      /* number of primes              */
static bool     initialized = false;

/* -------------------------------------------------------------------------- */
/*  Helper: compare two semip_t elements by 'prod'                            */
/* -------------------------------------------------------------------------- */
static int cmp_semip(const void *a, const void *b)
{
    const semip_t *sa = (const semip_t *)a;
    const semip_t *sb = (const semip_t *)b;
    if (sa->prod < sb->prod) return -1;
    if (sa->prod > sb->prod) return  1;
    return 0;
}

/* -------------------------------------------------------------------------- */
/*  Initialise tables: primes and all semiprimes up to LIMIT                 */
/* -------------------------------------------------------------------------- */
static void init(void)
{
    if (initialized) return;

    /* Limit large enough to contain > 10 000 semiprimes. */
    const uint32_t LIMIT = 500000;

    /* ---------------- Sieve of Eratosthenes for primes up to LIMIT ---------- */
    bool *sieve = (bool *)calloc((size_t)LIMIT + 1, sizeof(bool));
    if (!sieve) return;        /* allocation failed – abort silently */

    for (uint32_t i = 0; i <= LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= LIMIT; j += i) sieve[j] = false;
        }
    }

    /* Count primes and store them */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) ++cnt;
    }

    prime_cnt = cnt;
    primes    = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!primes) {
        free(sieve);
        return;
    }

    size_t k = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) primes[k++] = i;
    }
    free(sieve);

    /* ---------------- Generate all semiprimes <= LIMIT -------------------- */
    size_t cap = 128;
    semis      = (semip_t *)malloc(cap * sizeof(semip_t));
    if (!semis) return;

    semi_cnt = 0;
    for (size_t a = 0; a < prime_cnt; ++a) {
        for (size_t b = a + 1; b < prime_cnt; ++b) {
            uint64_t prod = (uint64_t)primes[a] * primes[b];
            if (prod > LIMIT) break;
            if (semi_cnt == cap) {
                cap *= 2;
                semis = (semip_t *)realloc(semis, cap * sizeof(semip_t));
                if (!semis) return;
            }
            semis[semi_cnt++] = (semip_t){prod, primes[a], primes[b]};
        }
    }

    /* Sort semiprimes by their product */
    qsort(semis, semi_cnt, sizeof(semip_t), cmp_semip);

    initialized = true;
}

/* -------------------------------------------------------------------------- */
/*  Sequence generator                                                        */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* no a(0) defined, return 0 */

    init();

    if ((idx_t)semi_cnt < n) return (seq_t)0;   /* out of range guard */

    const semip_t *sp = &semis[(size_t)n - 1];
    const uint32_t p = sp->p;
    const uint32_t q = sp->q;

    /* Minimal number with divisor count p*q:
     * 2^(q-1) * 3^(p-1)
     */
    seq_t part1 = pow2((idx_t)(q - 1));
    seq_t part2 = pow3((idx_t)(p - 1));
    return seq_mul(part1, part2);
}
