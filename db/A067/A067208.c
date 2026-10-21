/*
 * A067208  n is prime and remains prime when its leading digit is increased
 *          by one.
 * https://oeis.org/A067208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) is the n-th prime p such that p + 10^{digits(p)-1} is also
 *            prime
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Access to utility for generating primes and primality testing. */
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

static seq_t *cache = NULL;        /* Cached terms of the sequence.          */
static size_t cache_size = 0;      /* Number of terms stored.                 */
static size_t cache_cap  = 0;      /* Current cache capacity.                 */

static uint64_t *prime_list = NULL;   /* List of primes up to PRIMELIMIT.      */
static size_t    prime_cnt  = 0;       /* Number of primes.                     */
static size_t    prime_pos  = 0;       /* Current position in primes list.      */

static int once = 0;            /* Flag whether we have initialized.     */

/* Upper limit for primes used to generate the sequence.  
   Five million gives over 10 000 terms (≈22 000). */
#define PRIMELIMIT 5000000

/* Grow the cache to accommodate at least *need* terms. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
    while (new_cap < need) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(1);          /* exit on allocation failure. */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the prime list once. */
static void init_primes(void)
{
    if (once) return;
    build_primes_upto((idx_t)PRIMELIMIT, &prime_list, &prime_cnt);
    once = 1;
}

/* Append a new term to the cache. */
static void add_term(seq_t val)
{
    grow_cache(cache_size + 1);
    cache[cache_size++] = val;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for n < 1, but return 0.   */

    init_primes();          /* Ensure primes are ready.             */

    /* Generate terms until we have at least n of them. */
    while (cache_size < (size_t)n)
    {
        /* Fetch next prime p.  If we run out of primes, we would have to
           increase PRIMELIMIT, but PRIMELIMIT=5 000 000 suffices for n≤100 000. */
        if (prime_pos >= prime_cnt) break;   /* safety; should not happen */

        uint64_t p = prime_list[prime_pos++];
        /* Compute 10^{digits(p)-1}.  For a one‑digit prime, this is 1.   */
        uint64_t pow10 = 1;
        uint64_t temp = p;
        while (temp >= 10)
        {
            temp /= 10;
            pow10 *= 10;
        }

        uint64_t q = p + pow10;  /* The number obtained by increasing the leading digit. */
        if (is_prime((idx_t)q))
            add_term((seq_t)p);    /* p qualifies, store it. */
    }

    return cache[(size_t)n - 1];
}
