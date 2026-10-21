/*
 * A066522  Numbers m whose divisors less than or equal to sqrt(m) are
 *          consecutive, from 1 up to some number k.
 * https://oeis.org/A066522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A066522: numbers whose divisors ≤ √n are consecutive.
 * The sequence consists of all primes p, all numbers 2p where p is prime,
 * and the exceptional numbers 1, 8, 12, 18, 24, 60.
 * The function builds a list of primes once and then merges the three
 * sorted streams to produce the nth term.
 */

#include <stdint.h>
#include <stddef.h>
#include <limits.h>   /* UINT_MAX for the sieve size */

/*--- special numbers -----------------------------------------------------*/
static const seq_t specials[6] = {
    (seq_t)1, (seq_t)8, (seq_t)12, (seq_t)18, (seq_t)24, (seq_t)60
};

/*--- prime sieve ----------------------------------------------------------*/
/* Limit chosen large enough to provide several thousand primes for n up to
 * about 10 000 (the 10 000th prime is 104729).                      */
#define PRIME_LIMIT 200000           /* number of integers to sieve      */
#define MAX_PRIME_COUNT 20000       /* upper bound on #primes below limit */

static uint32_t primes[MAX_PRIME_COUNT];
static size_t prime_cnt = 0;
static int primes_ready  = 0;

static void build_primes(void)
{
    static uint8_t comp[PRIME_LIMIT + 1] = { 0 };   /* 0 → prime, 1 → composite */
    size_t i, j;

    for (i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            if (prime_cnt < MAX_PRIME_COUNT)
                primes[prime_cnt++] = (uint32_t)i;
            /* avoid overflow in i*i when i > sqrt(PRIME_LIMIT) */
            if (i * i <= PRIME_LIMIT) {
                for (j = i * i; j <= PRIME_LIMIT; j += i)
                    comp[j] = 1;
            }
        }
    }
    primes_ready = 1;
}

/*--- sequence generator ---------------------------------------------------*/
seq_t a(idx_t n)
{
    if (!primes_ready)
        build_primes();

    /* indices into the three streams */
    size_t sp  = 0;                /* specials    */
    size_t pi  = 0;                /* primes      */
    size_t di  = 0;                /* double primes */

    size_t produced = 0;           /* how many terms we have output so far */

    while (1) {
        seq_t best;
        int first = 1;             /* flag to initialise best */

        /* compare the current candidates from each stream */
        if (sp < 6) {
            if (first || specials[sp] < best) {
                best = specials[sp];
                first = 0;
            }
        }
        if (pi < prime_cnt) {
            seq_t cand = (seq_t)primes[pi];
            if (first || cand < best) {
                best = cand;
                first = 0;
            }
        }
        if (di < prime_cnt) {
            seq_t cand = (seq_t)primes[di] * (seq_t)2;
            if (first || cand < best) {
                best = cand;
                first = 0;
            }
        }

        /* advance the stream that produced the chosen value */
        if (sp < 6 && specials[sp] == best)
            ++sp;
        else if (pi < prime_cnt && (seq_t)primes[pi] == best)
            ++pi;
        else if (di < prime_cnt && (seq_t)primes[di] * (seq_t)2 == best)
            ++di;

        ++produced;
        if (produced == n)
            return best;
    }
}
