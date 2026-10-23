/*
 * A106552  Semiprime + (smallest integer obtained by concatenating its two
 *          divisors).
 * https://oeis.org/A106552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 2675, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th semiprime + min(concat(p,q), concat(q,p))
   where the semiprime is the product of two primes p ≤ q. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/concat_uint64.h"

typedef struct { uint64_t prod; uint32_t p; uint32_t q; } Semi;

static Semi *semis      = NULL;
static size_t semis_cnt = 0, semis_cap = 0;

static uint32_t *primes     = NULL;
static size_t primes_cnt   = 0;
static uint64_t sieve_lim  = 0;

/* ---------- prime generation ------------------------------------------- */
static void generate_primes(uint64_t limit)
{
    if (limit <= sieve_lim) return;              /* already done */

    uint8_t *sieve = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!sieve) abort();
    for (uint64_t i = 0; i <= limit; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (sieve[i])
            for (uint64_t j = i * i; j <= limit; j += i) sieve[j] = 0;

    /* collect primes */
    size_t cap = 0;
    free(primes);
    primes = NULL;
    primes_cnt = 0;          /* reset count */
    for (uint64_t i = 2; i <= limit; ++i)
        if (sieve[i]) {
            if (primes_cnt == cap) {
                cap = cap ? cap * 2 : 256;
                uint32_t *tmp = (uint32_t*)realloc(primes, cap * sizeof(uint32_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    free(sieve);
    sieve_lim = limit;
}

/* ---------- semiprime generation --------------------------------------- */
static int cmp_semis(const void *a, const void *b)
{
    const Semi *sa = (const Semi*)a, *sb = (const Semi*)b;
    if (sa->prod < sb->prod) return -1;
    if (sa->prod > sb->prod) return 1;
    return 0;
}

static void push_semi(uint64_t prod, uint32_t p, uint32_t q)
{
    if (semis_cnt == semis_cap) {
        semis_cap = semis_cap ? semis_cap * 2 : 256;
        Semi *tmp = (Semi*)realloc(semis, semis_cap * sizeof(Semi));
        if (!tmp) abort();
        semis = tmp;
    }
    semis[semis_cnt++] = (Semi){prod, p, q};
}

static void build_semiprimes(uint64_t limit)
{
    free(semis);
    semis = NULL;
    semis_cap = semis_cnt = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        for (size_t j = i; j < primes_cnt; ++j) {
            uint64_t q = primes[j];
            uint64_t prod = p * q;
            if (prod > limit) break;
            push_semi(prod, (uint32_t)p, (uint32_t)q);
        }
    }
    qsort(semis, semis_cnt, sizeof(Semi), cmp_semis);
}

/* ensure we have at least n semiprimes built */
static void ensure_semis(idx_t n)
{
    static uint64_t limit = 10;
    while (semis_cnt < (size_t)n) {
        limit *= 2;
        generate_primes(limit);
        build_semiprimes(limit);
    }
}

/* ---------- the sequence ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_semis(n);
    const Semi *s = &semis[n - 1];
    seq_t c1 = concat_uint64(s->p, s->q);
    seq_t c2 = concat_uint64(s->q, s->p);
    seq_t concat_min = c1 < c2 ? c1 : c2;
    return (seq_t)s->prod + concat_min;
}
