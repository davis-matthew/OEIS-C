/*
 * A178502  Numbers n such that prime(n) modulo n is less than n/2.
 * https://oeis.org/A178502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that prime(n) modulo n is less than n/2. */
/* This implementation builds the sequence on demand by generating primes
   with a simple sieve and caching all found terms.  The function is
   memoised and works for any combination of seq_t and idx_t defined
   by the build around this file. */
 
#include <stdlib.h>
#include <stdbool.h>

static seq_t *terms      = NULL;   /* cached sequence terms          */
static size_t terms_len  = 0;      /* number of terms cached         */
static size_t terms_cap  = 0;      /* allocated capacity             */

static uint32_t *primes   = NULL;  /* list of primes as uint32_t     */
static size_t primes_len  = 0;     /* number of primes cached        */
static size_t primes_cap  = 0;     /* allocated capacity for primes  */
static size_t prime_limit = 0;     /* numeric upper bound for sieve  */

static idx_t next_candidate = 1;   /* next integer to test           */

/* Simple sieve to collect all primes <= limit.  */
static void generate_primes_upto(size_t limit)
{
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_comp) exit(1);

    size_t count = 0;
    for (size_t p = 2; p <= limit; ++p) {
        if (!is_comp[p]) {
            if (count == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 64;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[count++] = (uint32_t)p;

            if (p * p <= limit) {
                for (size_t m = p * p; m <= limit; m += p)
                    is_comp[m] = true;
            }
        }
    }
    free(is_comp);
    primes_len = count;
    prime_limit = limit;
}

/* Ensure that at least 'needed' primes are available (1‑based index). */
static void ensure_primes_count(size_t needed)
{
    if (needed <= primes_len) return;

    while (primes_len < needed) {
        /* Choose a new numeric limit large enough to likely contain 'needed' primes. */
        size_t new_limit = prime_limit ? prime_limit * 2 : 1000;
        /* If prime_limit is 0, ensure we use a positive limit. */
        if (new_limit <= prime_limit) new_limit = prime_limit + 1000;

        /* Generate primes up to the new limit. */
        free(primes);
        primes = NULL;
        primes_cap = 0;
        primes_len = 0;
        generate_primes_upto(new_limit);
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined but defensive */

    /* Extend the cached sequence until we have at least the requested index. */
    while (terms_len < (size_t)n) {
        /* Make sure the prime for the current candidate exists. */
        ensure_primes_count((size_t)next_candidate);

        seq_t pn = (seq_t)primes[(size_t)next_candidate - 1];
        seq_t cn = (seq_t)next_candidate;
        seq_t rem = pn % cn;               /* prime(n) mod n */

        /* Condition: prime(n) mod n < n/2  <=>  2*(prime(n) mod n) < n */
        if ((rem << 1) < cn) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                terms = tmp;
            }
            terms[terms_len++] = cn;
        }
        ++next_candidate;
    }
    return terms[(size_t)n - 1];
}
