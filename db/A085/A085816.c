/*
 * A085816  Semiprimes such that the sum of the factorials of the digits is
 *          also semiprime.
 * https://oeis.org/A085816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = n-th semiprime p such that the sum of the factorials of
 *            the digits of p is also a semiprime
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code builds a list of all such numbers up to a fixed limit (3 million) once, then answers
   queries by indexing into that list.  It uses a linear sieve to compute the number of prime
   factors (with multiplicity) for every integer up to the limit, which allows immediate
   recognition of semiprimes and of semiprime factorial‑digit sums. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* factorial values for digits 0–9 */
static const seq_t FACT[10] = {
    (seq_t)1,   /* 0! */
    (seq_t)1,   /* 1! */
    (seq_t)2,   /* 2! */
    (seq_t)6,   /* 3! */
    (seq_t)24,  /* 4! */
    (seq_t)120, /* 5! */
    (seq_t)720, /* 6! */
    (seq_t)5040,/* 7! */
    (seq_t)40320,/* 8! */
    (seq_t)362880 /* 9! */
};

/* ---------- static data used for one‑time initialisation ---------- */
#define LIMIT 3000000UL   /* enough to contain the first 10 000 terms */

static uint32_t *lp;          /* lowest prime factor */
static uint8_t  *omega;       /* number of prime factors with multiplicity */
static uint32_t *primes;      /* list of primes */
static size_t   prime_cnt = 0;

static seq_t *terms = NULL;   /* list of valid terms */
static size_t terms_size = 0;
static size_t terms_cap  = 0;

/* ----------------------------------------------------------------- */
/* Perform a linear sieve to compute lp[] and omega[] up to LIMIT    */
/* ----------------------------------------------------------------- */
static void init_sieve(void)
{
    if (lp) return;           /* already initialised */

    /* allocate helpers */
    lp     = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    omega  = (uint8_t*)calloc((LIMIT + 1), sizeof(uint8_t));
    size_t prime_cap = LIMIT / 10 + 1024;      /* generous over‑estimate */
    primes = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));

    /* linear sieve */
    prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (lp[i] == 0) {                 /* i is prime */
            lp[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
            omega[i] = 1;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            if (p > lp[i]) break;
            size_t v = i * (size_t)p;
            if (v > LIMIT) break;
            lp[v] = (uint32_t)p;
            omega[v] = omega[i] + 1;
        }
    }

    /* build the sequence vector */
    terms_cap = 128;
    terms     = (seq_t*)malloc(terms_cap * sizeof(seq_t));

    for (size_t i = 4; i <= LIMIT; ++i) {
        if (omega[i] != 2) continue;         /* not a semiprime */

        /* sum of factorials of the digits of i */
        seq_t sum = 0;
        seq_t tmp = (seq_t)i;
        while (tmp) {
            uint8_t d = (uint8_t)(tmp % 10);
            sum += FACT[d];
            tmp /= 10;
        }

        if (omega[(size_t)sum] != 2) continue;  /* factorial‑sum not semiprime */

        /* store the value */
        if (terms_size == terms_cap) {
            terms_cap <<= 1;
            terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
        }
        terms[terms_size++] = (seq_t)i;
    }
}

/* ----------------------------------------------------------------- */
/*   required sequence function                                       */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* behaviour for n < 1 is irrelevant */

    init_sieve();                  /* one‑time initialisation */

    /* safety check – should never be required for the given limits */
    if ((size_t)(n - 1) >= terms_size) {
        /* not enough terms precomputed – extend the limit if necessary.
           For the problem constraints (n ≤ 10000) this branch is never hit. */
        return (seq_t)0;
    }

    return terms[n - 1];
}
