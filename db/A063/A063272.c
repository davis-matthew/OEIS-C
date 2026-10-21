/*
 * A063272  Number of times most common final digit of primes appears in first
 *          n primes.
 * https://oeis.org/A063272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of times the most common last digit among the
 *            first n primes appears within those n primes. It is computed
 *            incrementally by extending the list of primes and keeping the
 *            current maximum frequency
 * Range:     u8 n <= 1002, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Static state used to compute the sequence incrementally                 */
static idx_t primes_count   = 0;                 /* how many primes are currently stored      */
static uint64_t *primes = NULL;                /* array of primes                         */
static size_t primes_cap = 0;                  /* allocated capacity for primes array      */

static seq_t *a_vals = NULL;                  /* cached a(i) values up to primes_count   */
static size_t a_cap  = 0;                     /* allocated capacity for a_vals           */

static seq_t digit_counts[10] = {0};           /* frequency of each final digit 0‑9       */
static seq_t max_freq            = 0;          /* current maximum frequency              */

static uint64_t next_candidate = 2;            /* next number to test for primality      */

/* --------------------------------------------------------------------- */
/* Grow the primes array if needed                                         */
static void grow_primes(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap * 2 : 32;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp) abort();   /* out of memory */
    primes   = tmp;
    primes_cap = new_cap;
}

/* Grow the a_vals array if needed                                         */
static void grow_a_vals(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 32;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* out of memory */
    a_vals = tmp;
    a_cap  = new_cap;
}

/* --------------------------------------------------------------------- */
/* Simple primality test using already stored primes (trial division)    */
static bool is_prime_candidate(uint64_t cand)
{
    for (size_t i = 0; i < primes_count; ++i) {
        uint64_t p = primes[i];
        if ((unsigned __int128)p * p > cand) break;
        if (cand % p == 0) return false;
    }
    return true;
}

/* --------------------------------------------------------------------- */
/* Extend the precomputation up to at least n primes                      */
static void extend_to(idx_t n)
{
    while (primes_count < n) {
        /* Find next prime candidate */
        uint64_t cand = next_candidate;
        if (cand < 2) cand = 2;
        if (cand > 2 && (cand & 1ULL) == 0) cand++;   /* skip even numbers */

        while (!is_prime_candidate(cand)) {
            cand += 2;                                 /* only odd values */
        }

        /* Store the new prime */
        grow_primes(primes_count + 1);
        primes[primes_count] = cand;
        ++primes_count;

        /* Update digit statistics */
        int d = cand % 10;
        ++digit_counts[d];
        if (digit_counts[d] > max_freq) max_freq = digit_counts[d];

        /* Record a(n) for this prime count */
        grow_a_vals(primes_count);
        a_vals[primes_count - 1] = max_freq;

        /* Prepare next candidate */
        next_candidate = (cand == 2) ? 3 : cand + 2;
    }
}

/* --------------------------------------------------------------------- */
/* Public interface: return a(n) for n >= 1                              */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined outside the offset */
    extend_to(n);
    return a_vals[n - 1];
}
