/*
 * A115093  Primes of the form p*q-2, where p and q are distinct primes.
 * https://oeis.org/A115093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A115093: primes p such that p+2 = a*b with distinct primes a,b. */

#include <stdint.h>
#include <stdlib.h>

/* ---------- static state ---------- */
static seq_t *seq_buf = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;

static uint8_t  *is_comp = NULL;
static uint32_t *spf     = NULL;
static size_t  sieve_limit = 0;
static size_t  last_prime_index = 0;

/* ---------- helpers ---------- */
static void build_sieve(size_t limit)
{
    /* free any previous allocation */
    free(is_comp);
    free(spf);
    free(primes);

    is_comp = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
    spf     = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (is_comp == NULL || spf == NULL) return;

    /* initialise */
    for (size_t i = 0; i <= limit; ++i) {
        is_comp[i] = 0;
        spf[i]     = 0;
    }

    /* allocate primes array */
    size_t prime_cap = limit / 10 + 16;
    primes = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (primes == NULL) return;
    primes_cnt = 0;

    /* linear sieve for primes and spf */
    for (size_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == prime_cap) {
                /* grow primes array if needed */
                size_t new_cap = prime_cap * 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (tmp == NULL) return;
                primes = tmp;
                prime_cap = new_cap;
            }
            primes[primes_cnt++] = (uint32_t)i;
            spf[i] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint64_t v = (uint64_t)i * primes[j];
            if (v > limit) break;
            is_comp[(size_t)v] = 1;
            if (spf[(size_t)v] == 0) spf[(size_t)v] = primes[j];
            if (i % primes[j] == 0) break;
        }
    }
    sieve_limit = limit;
}

static void ensure_limit(size_t limit)
{
    if (limit > sieve_limit) build_sieve(limit);
}

static void extend_sequence(size_t need)
{
    if (seq_len >= need) return;

    size_t limit = sieve_limit ? sieve_limit : 200000;
    if (limit < 200000) limit = 200000;

    while (seq_len < need) {
        ensure_limit(limit);

        for (; last_prime_index < primes_cnt && seq_len < need; ++last_prime_index) {
            uint32_t p = primes[last_prime_index];
            if (p <= 2u) continue;            /* 2 gives 4 = 2*2, not distinct */

            uint64_t x = (uint64_t)p + 2u;     /* candidate p+2 */
            if (x > limit + 2u) continue;     /* safety guard */

            uint32_t f = spf[(size_t)x];
            if (f == (uint32_t)x) continue;   /* x is prime */

            uint32_t g = (uint32_t)(x / f);
            if (g == f) continue;             /* square of a prime */

            if (spf[(size_t)g] != g) continue; /* g not prime */

            /* accept p */
            if (seq_len == seq_cap) {
                seq_cap = seq_cap ? seq_cap * 2 : 16;
                seq_buf = (seq_t *)realloc(seq_buf, seq_cap * sizeof(seq_t));
            }
            seq_buf[seq_len++] = (seq_t)p;
        }

        if (seq_len >= need) break;

        /* need more primes: double the limit and continue */
        limit *= 2;
        if (limit == 0) limit = SIZE_MAX; /* guard against overflow */
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* behaviour below offset is irrelevant */

    size_t sn = (size_t)n;
    if (seq_len < sn) extend_sequence(sn);

    return seq_buf[sn - 1];
}
