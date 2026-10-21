/*
 * A213997  Number of integers between successive prime triples (p, p+2, p+6).
 * https://oeis.org/A213997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A213997: Number of integers between successive prime triples (p, p+2, p+6).
   Formula: a(n) = max(0, p_{n+1} - p_n - 7) where p_k is the k-th start of a prime triple.
   Implementation: a(n) is computed by generating the list of starting primes of all
   prime triples up to the required index.  The list is grown lazily and cached
   across calls.  Primality is tested by a simple sieve rebuilt whenever a larger
   limit is needed.  The algorithm is O(N log log N) for generating the first N
   triples.  It satisfies the performance constraints for n up to ~10,000. */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/*  Helper data structures and dynamic memory usage                     */
/* --------------------------------------------------------------------- */

/* Store the starting primes of prime triples. */
static idx_t *triples      = NULL;   /* array of start primes */
static idx_t  triples_cnt  = 0;      /* number of elements in triples array */
static idx_t  triples_cap  = 0;      /* allocated capacity */

/* Boolean sieve marking primality (1 = prime, 0 = composite). */
static unsigned char *is_prime   = NULL;   /* array of 1/0 for each integer */
static idx_t          sieve_lim = 0;      /* current upper bound of the sieve */

/* Current candidate odd number being tested for a triple. */
static idx_t candidate = 3;

/* --------------------------------------------------------------------- */
/*  Sieve utilities                                                    */
/* --------------------------------------------------------------------- */

static void rebuild_sieve(idx_t limit)
{
    /* Allocate new sieve array and mark all as prime initially. */
    unsigned char *new_arr = (unsigned char*)malloc((limit + 1) * sizeof(unsigned char));
    for (idx_t i = 0; i <= limit; ++i)
        new_arr[i] = 1;                 /* assume prime */

    new_arr[0] = new_arr[1] = 0;         /* 0 and 1 are not prime */

    idx_t sq = (idx_t)sqrt((double)limit);
    for (idx_t i = 2; i <= sq; ++i)
        if (new_arr[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                new_arr[j] = 0;
        }

    /* Replace old array and free memory. */
    free(is_prime);
    is_prime = new_arr;
    sieve_lim = limit;
}

static void enlarge_sieve(idx_t min_needed)
{
    /* Ensure the sieve covers at least min_needed. */
    if (min_needed <= sieve_lim)
        return;

    /* Increase the limit by doubling until it exceeds min_needed. */
    idx_t new_lim = sieve_lim ? sieve_lim : 1;
    while (new_lim < min_needed)
        new_lim *= 2;

    rebuild_sieve(new_lim);
}

/* --------------------------------------------------------------------- */
/*  Triple list building                                               */
/* --------------------------------------------------------------------- */

static void ensure_triples(idx_t n)
{
    /* Need triples[0..n] (i.e., n+1 elements). */
    while (triples_cnt <= n) {
        enlarge_sieve(candidate + 6);

        if (is_prime[candidate] &&
            is_prime[candidate + 2] &&
            is_prime[candidate + 6]) {

            /* Grow the triples array if necessary. */
            if (triples_cap == 0) {
                triples_cap = 8;
                triples = (idx_t*)malloc(triples_cap * sizeof(idx_t));
            }
            if (triples_cnt == triples_cap) {
                triples_cap *= 2;
                triples = (idx_t*)realloc(triples, triples_cap * sizeof(idx_t));
            }
            triples[triples_cnt++] = candidate;
        }
        candidate += 2;   /* next odd number */
    }
}

/* --------------------------------------------------------------------- */
/*  Main sequence function                                            */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* The sequence is 1-indexed: a(1) uses triples[0] and triples[1]. */
    if (n == 0)
        return 0;   /* undefined, but guard against underflow */

    ensure_triples(n);   /* guarantees triples[0..n] exist */

    /* Compute difference between successive triple starts. */
    if (triples[n] > triples[n-1] + 7)
        return (seq_t)(triples[n] - triples[n-1] - 7);
    else
        return (seq_t)0;
}
