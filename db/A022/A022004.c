/*
 * A022004  Initial members of prime triples (p, p+2, p+6).
 * https://oeis.org/A022004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 183, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Prime triples (p, p+2, p+6).  a(n) is the first prime p
 * such that p, p+2, p+6 are all prime; OEIS A022004.  The sequence
 * is indexed from 1; a(1) = 5.  The function computes the
 * sequence by sieving the integers up to a fixed limit that is
 * sufficient for all n ≤ 10000 (limit = 50 000 000).  The results
 * are stored in a static array for fast repeated access.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build system. */
seq_t a(idx_t n)
{
    /* Handle indexing from 1; zero is a dummy value. */
    if (n == 0) return (seq_t)0;

    static bool initialized = false;
    static uint64_t *triples = NULL;   /* array of primes p */
    static size_t triple_cnt = 0;      /* number of found triples */

    if (!initialized) {
        /* ------------------------------------------------------------
           Pre‑compute all prime triples up to LIMIT.
           LIMIT is chosen large enough for n ≤ 10000 and
           requires about 25 MiB of memory (only odd indices).
           ------------------------------------------------------------ */
        const uint64_t LIMIT = 50000000ULL;          /* 5×10⁷ */
        const size_t SB_SIZE = (LIMIT >> 1) + 1;     /* odd indices only */
        uint8_t *sieve = (uint8_t *)calloc(SB_SIZE, 1); /* 0 = prime */

        /* Sieve of Eratosthenes for odd numbers only.
         * sieve[i] == 0  <=>  (2*i+1) is prime.
         */
        for (uint64_t i = 3; i * i <= LIMIT; i += 2) {
            if (!sieve[i >> 1]) {            /* i is prime */
                for (uint64_t j = i * i; j <= LIMIT; j += i << 1)
                    sieve[j >> 1] = 1;         /* mark composite */
            }
        }

        /* Scan for prime triples and store the starting prime p. */
        size_t cap = 20000;                  /* initial capacity */
        triples = (uint64_t *)malloc(cap * sizeof(uint64_t));
        triple_cnt = 0;

        for (uint64_t p = 5; p <= LIMIT - 6; p += 2) {
            if (!sieve[p >> 1]) {                /* p is prime */
                uint64_t p2 = p + 2;
                uint64_t p6 = p + 6;
                if (!sieve[p2 >> 1] && !sieve[p6 >> 1]) { /* p+2 & p+6 prime */
                    if (triple_cnt == cap) {      /* grow array */
                        cap *= 2;
                        triples = (uint64_t *)realloc(triples,
                                                       cap * sizeof(uint64_t));
                    }
                    triples[triple_cnt++] = p;
                }
            }
        }

        free(sieve);
        initialized = true;
    }

    /* Return the n‑th term if it exists, otherwise 0. */
    if (n <= triple_cnt)
        return (seq_t)triples[n - 1];
    else
        return (seq_t)0;
}
