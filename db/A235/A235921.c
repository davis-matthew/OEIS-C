/*
 * A235921  Numbers n such that smallest number not dividing n^2 (A236454) is
 *          different from smallest prime not dividing n (A053669).
 * https://oeis.org/A235921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 202, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: finds the nth number n such that the smallest number not
 * dividing n^2 (A236454) differs from the smallest prime not dividing n
 * (A053669).  All terms are multiples of 210, so we search those multiples
 * in increasing order until we have met n such numbers. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime sieve limit (enough to find the first missing prime for n up to
 * 2.1e6, the 10000th term). */
#define PRIME_LIMIT 20000

static uint64_t primes[PRIME_LIMIT];
static size_t primes_cnt = 0;
static bool primes_ready = false;

/* Build prime list once. */
static void init_primes(void) {
    if (primes_ready) return;
    bool *mark = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!mark) exit(1); /* abort on allocation failure */
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) mark[i] = true;
    mark[0] = mark[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (mark[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                mark[j] = false;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (mark[i]) primes[primes_cnt++] = i;
    }
    free(mark);
    primes_ready = true;
}

/* Returns the nth term of sequence A235921 (1‑based index). */
seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n<1, but guard anyway */

    init_primes();

    size_t target = (size_t)n;   /* nth term to find */
    size_t found = 0;            /* how many satisfied so far */

    for (idx_t k = 1;; ++k) {
        /* Candidate n = 210 * k */
        seq_t cand = (seq_t)210 * (seq_t)k;
        seq_t cand_sq = cand * cand;

        /* Smallest number not dividing cand^2 : A236454 */
        seq_t m = 1;
        for (;; ++m) {
            if (cand_sq % m != 0) break;
        }

        /* Smallest prime not dividing cand : A053669 */
        seq_t missing_prime = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            seq_t p = (seq_t)primes[i];
            if (cand % p != 0) {
                missing_prime = p;
                break;
            }
        }
        /* The missing_prime should always be found within the sieve limit
         * for the range of interest. */

        if (missing_prime != m) {
            ++found;
            if (found == target) return cand;
        }
    }
}
