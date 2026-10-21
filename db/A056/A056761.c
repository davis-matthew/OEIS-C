/*
 * A056761  Odd numbers less than the cube of their number of divisors.
 * https://oeis.org/A056761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 216, u32 n <= 267, u64 n <= 267,
 *            u128 n <= 267, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the nth odd integer x such that x < d(x)^3,
   where d(x) is the number of divisors of x. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/*  Sieve for divisor counts (linear sieve)                           */
/* ------------------------------------------------------------------ */
#define MAX_LIMIT 2000000U      /* large enough to cover all terms */

static uint32_t *divcnt  = NULL; /* divisor counts */
static uint32_t *exp_arr = NULL; /* exponent of spf in each number */
static uint32_t *spf     = NULL; /* smallest prime factor */

static uint32_t *primes  = NULL; /* list of primes */
static size_t   primes_cnt = 0;   /* number of primes found */

static void build_sieve(void)
{
    if (divcnt != NULL)            /* already built */
        return;

    size_t N = MAX_LIMIT;

    divcnt  = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    exp_arr = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    spf     = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    primes  = (uint32_t *)calloc(N / 2 + 1, sizeof(uint32_t)); /* roughly enough */

    if (!divcnt || !exp_arr || !spf || !primes)
        abort();

    divcnt[1]  = 1;
    exp_arr[1] = 0;
    spf[1]     = 1;

    for (uint32_t i = 2; i <= N; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i]      = i;
            exp_arr[i] = 1;
            divcnt[i]  = 2;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t ip = (uint64_t)i * p;
            if (ip > N)
                break;
            spf[ip] = p;
            if (p == spf[i]) {        /* p divides i */
                exp_arr[ip] = exp_arr[i] + 1;
                divcnt[ip]  = divcnt[i] / (exp_arr[i] + 1) * (exp_arr[ip] + 1);
            } else {                  /* p does not divide i */
                exp_arr[ip] = 1;
                divcnt[ip]  = divcnt[i] * 2;
            }
            if (p == spf[i])      /* linear sieve condition */
                break;
        }
    }
}

/* ------------------------------------------------------------------ */
/*  Cache of sequence terms                                             */
/* ------------------------------------------------------------------ */
static seq_t *terms   = NULL;   /* cached terms */
static size_t terms_cap   = 0;  /* allocated capacity */
static size_t terms_size  = 0;  /* number of cached terms */

/* Ensure the terms array has space for 'needed' elements. */
static void assure_terms(size_t needed)
{
    if (terms_cap >= needed)
        return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    terms = tmp;
    terms_cap = new_cap;
}

/* Generate all terms of the sequence and cache them. */
static void generate_terms(void)
{
    if (terms_size > 0)
        return;                     /* already generated */

    build_sieve();                  /* prepare divisor counts */

    for (uint32_t num = 3; num <= MAX_LIMIT; num += 2) { /* odd numbers only */
        uint32_t d = divcnt[num];
        seq_t d3 = (seq_t)d * d * d;
        if ((seq_t)num < d3) {
            assure_terms(terms_size + 1);
            terms[terms_size++] = (seq_t)num;
        }
    }
    /* The sequence stops after the last such odd number within the limit. */
}

/* ------------------------------------------------------------------ */
/*  Public sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; undefined below */
        return 0;

    generate_terms();

    if ((size_t)n > terms_size)
        return 0; /* undefined for indices beyond the end of the sequence */

    return terms[(size_t)n - 1];
}
