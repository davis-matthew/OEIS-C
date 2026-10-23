/*
 * A141793  Nonprime positive integers of the form k/(prime(k+2)-prime(k+1))
 *          in order of increasing k.
 * https://oeis.org/A141793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = k / (prime(k+2) - prime(k+1)) for the n‑th k such that k
 *            is divisible by the denominator and the quotient is a nonprime
 *            positive integer. (The sequence is generated in increasing order
 *            of the index k.)
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   build_primes_upto, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto */
#include "../utils/is_prime_big.h"        /* is_prime_big */
#include "../utils/ensure_capacity.h"    /* ensure_capacity */

/* static state shared by all calls */
static seq_t *term_cache  = NULL;   /* cache of computed terms          */
static size_t term_len   = 0;       /* number of terms stored           */
static size_t term_cap   = 0;       /* allocated cache capacity         */

static uint64_t *primes  = NULL;    /* array of primes (prime(1)=2)     */
static size_t    primes_cnt = 0;    /* number of primes in the array    */

static idx_t next_k = 1;           /* next index k to examine          */
static idx_t prime_limit = 20000000; /* initial sieve limit (value)     */

/* initialize prime array once */
static void init_primes(void)
{
    if (primes != NULL)
        return;

    build_primes_upto(prime_limit, &primes, &primes_cnt);

    /* In the unlikely event the limit was insufficient for the
       requested n, we would have to rebuild with a larger limit
       (omitted for brevity).  For n up to ~10000 the chosen limit
       is more than enough. */
}

/* ensure the cache can hold at least `need` elements */
static void ensure_term_cache(size_t need)
{
    if (term_cap >= need)
        return;
    ensure_capacity(need, &term_cap, (void**)&term_cache, sizeof(seq_t));
}

/* returns the n-th term (n >= 1), with caching and lazy prime generation */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined, safe return */

    /* fast path if already computed */
    if (term_len >= (size_t)n)
        return term_cache[(size_t)n - 1];

    init_primes();                      /* ensure prime list is ready */

    while (term_len < (size_t)n) {
        /* guard against running out of primes (unlikely for n <= 10000) */
        if ((size_t)(next_k + 1) >= primes_cnt) {
            /* enlarge sieve limit and rebuild */
            prime_limit *= 2;
            free(primes);
            build_primes_upto(prime_limit, &primes, &primes_cnt);
        }

        idx_t diff = primes[next_k + 1] - primes[next_k];
        if (diff != 0 && next_k % diff == 0) {
            seq_t val = (seq_t)(next_k / diff);
            if (!is_prime_big(val)) {           /* w.r.t. seq_t type   */
                ensure_term_cache(term_len + 1);
                term_cache[term_len++] = val;
            }
        }
        ++next_k;
    }
    return term_cache[(size_t)n - 1];
}
