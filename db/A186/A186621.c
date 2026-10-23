/*
 * A186621  Semiprimes - 1.
 * https://oeis.org/A186621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A001358(n) - 1 (semiprime of index n, minus 1)
 * Range:     u8 n <= 82, u16 n <= 1366, u32 n <= 1366, u64 n <= 1366,
 *            u128 n <= 1366, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Upper bound for the sieve.  4 000 000 is enough to generate
   the first 100 000 semiprimes, which covers the required range
   of n ≈ 10 000.  Adjust if larger n are needed.               */
static const idx_t LIMIT = 4000000UL;

/* Smallest prime factor table */
static uint32_t *spf = NULL;

/* Cache of semiprime values (type seq_t) */
static seq_t *semiprimes = NULL;
static size_t semiprimes_cnt  = 0;
static size_t semiprimes_cap  = 0;

/* Highest number examined so far */
static uint32_t max_generated = 1;

/* One‑time initialization */
static void init(void)
{
    if (spf != NULL)            /* already initialised */
        return;

    /* ----- build smallest prime factor array ----- */
    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    memset(spf, 0, (LIMIT + 1) * sizeof(uint32_t));

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i) {
                    if (spf[(size_t)j] == 0)
                        spf[(size_t)j] = i;
                }
            }
        }
    }

    /* ----- reserve space for first few semiprimes ----- */
    semiprimes_cap = 200000;                 /* arbitrary initial size */
    semiprimes = (seq_t *)malloc(semiprimes_cap * sizeof(seq_t));
    semiprimes_cnt = 0;
}

/* Return true iff x is a semiprime (product of exactly two primes). */
static inline bool is_semiprime(uint32_t x)
{
    if (x < 4)
        return false;          /* smallest semiprime is 4 */

    uint32_t p = spf[x];
    if (p == 0)                 /* safety, should never happen */
        p = x;

    uint32_t y = x / p;         /* second factor */

    if (y == 1)                 /* x itself was prime */
        return false;

    /* x has exactly two prime factors iff the second factor is prime. */
    return spf[y] == y;
}

/* Grow the semiprime cache dynamically. */
static void grow_semiprimes(void)
{
    if (semiprimes_cnt < semiprimes_cap)
        return;
    semiprimes_cap *= 2;
    semiprimes = (seq_t *)realloc(semiprimes, semiprimes_cap * sizeof(seq_t));
}

/* ----------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)        /* offset does not matter */
        return (seq_t)0;

    init();

    /* Generate semiprimes until we have at least n of them. */
    while (semiprimes_cnt < (size_t)n) {
        ++max_generated;
        if (max_generated > LIMIT) {
            /* Should never happen for the intended n. */
            return (seq_t)0;
        }
        if (is_semiprime((uint32_t)max_generated)) {
            grow_semiprimes();
            semiprimes[semiprimes_cnt++] = (seq_t)max_generated;
        }
    }

    /* The sequence is semiprime minus 1. */
    return semiprimes[(size_t)n - 1] - (seq_t)1;
}
