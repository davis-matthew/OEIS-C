/*
 * A095963  If p(x) is the x-th prime, then the n-th set of 4 consecutive sexy
 *          prime pairs starts at p(a(n)).
 * https://oeis.org/A095963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the index of the first prime of the n‑th block of four
   consecutive sexy prime pairs.  No simple formula is known;
   the values are obtained by searching primes and testing the
   pattern described in OEIS A095963. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static unsigned char *prime_sieve = NULL;   /* bitset of composites   */
static idx_t sieve_limit = 0;              /* current sieve upper bound      */
static idx_t *start_indices = NULL;        /* array of a(n) indices        */
static size_t starts_len = 0;              /* number of values computed     */
static size_t starts_cap = 0;              /* allocated capacity            */

/* Simple bitset utilities */
#define IS_COMPOSITE(n)  (prime_sieve[(n) >> 3] & (1 << ((n) & 7)))
#define SET_COMPOSITE(n) (prime_sieve[(n) >> 3] |= (1 << ((n) & 7)))

/* Ensure that the starts array has capacity for at least 'need' entries */
static void ensure_start_capacity(size_t need)
{
    if (starts_cap < need) {
        size_t new_cap = starts_cap ? starts_cap << 1 : 8;
        while (new_cap < need) new_cap <<= 1;
        idx_t *tmp = (idx_t *)realloc(start_indices,
                                      new_cap * sizeof(idx_t));
        start_indices = tmp;
        starts_cap = new_cap;
    }
}

/* Recompute the entire sieve and the list of qualifying starts up to
   the new 'limit'.  The previous sieve and start list are discarded. */
static void recompute_sieve_and_starts(idx_t limit)
{
    /* free old resources */
    free(prime_sieve);
    free(start_indices);
    prime_sieve = NULL;
    start_indices = NULL;
    sieve_limit = limit;
    starts_len = 0;
    starts_cap = 0;

    /* allocate bitset: one bit per number up to 'limit' */
    size_t sz = (limit >> 3) + 1;
    prime_sieve = (unsigned char *)malloc(sz);
    if (!prime_sieve) return;
    memset(prime_sieve, 0, sz);            /* all numbers initially prime */

    /* Sieve of Eratosthenes */
    idx_t i, j;
    for (i = 2; (idx_t)i * (idx_t)i <= limit; ++i)
        if (!IS_COMPOSITE(i))
            for (j = i * i; j <= limit; j += i)
                SET_COMPOSITE(j);

    /* Scan prime numbers, collecting qualifying starts.
       Maintain the last 8 primes in a sliding window. */
    uint64_t last[8] = {0};
    idx_t primes_seen = 0;                /* count of primes discovered */

    for (i = 2; i <= limit; ++i) {
        if (IS_COMPOSITE(i)) continue;    /* skip composites */

        primes_seen++;
        /* shift the sliding window */
        for (size_t k = 7; k > 0; --k)
            last[k] = last[k-1];
        last[0] = (uint64_t)i;

        if (primes_seen >= 8) {
            if (last[0] - last[1] == 6 &&
                last[2] - last[3] == 6 &&
                last[4] - last[5] == 6 &&
                last[6] - last[7] == 6) {
                ensure_start_capacity(starts_len + 1);
                start_indices[starts_len++] = primes_seen - 7;
            }
        }
    }
}

/* Main public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */

    /* Ensure we have enough precomputation to answer this query. */
    while (starts_len < (size_t)n) {
        /* Initial limit or double the current one each time. */
        idx_t new_limit = sieve_limit ? sieve_limit << 1 : 1000000ULL;
        /* Prevent overflow: keep new_limit at least as large as before. */
        if (new_limit <= sieve_limit) new_limit = sieve_limit;
        recompute_sieve_and_starts(new_limit);
    }

    return (seq_t)start_indices[n - 1];
}
