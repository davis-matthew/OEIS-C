/*
 * A125903  a(n) = product of the first n primes which are coprime to n.
 * https://oeis.org/A125903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the first n primes that are coprime to n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   build_primes_upto, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper function declarations provided by the build environment */
#include "../utils/build_primes_upto.h"
#include "../utils/seq_gcd.h"

/* Static cache for primes */
static uint64_t *prime_list   = NULL;  /* array of primes */
static size_t   prime_cnt    = 0;      /* number of primes in the cache */
static idx_t    prime_limit  = 0;      /* limit used for the last sieve */

/* Ensure that at least 'min_needed' primes are cached.
   If not, generate primes up to a larger limit until enough are found. */
static void expand_prime_cache(size_t min_needed)
{
    if (prime_cnt >= min_needed)
        return;

    idx_t limit = prime_limit ? prime_limit * 2 : 1000;
    while (1) {
        uint64_t *buf = NULL;
        size_t    cnt = 0;
        build_primes_upto(limit, &buf, &cnt);

        if (cnt >= min_needed) {
            /* Free previous list and store the new one */
            free(prime_list);
            prime_list = buf;
            prime_cnt  = cnt;
            prime_limit = limit;
            break;
        }
        /* Not enough primes yet – try a larger limit */
        free(buf);
        limit <<= 1;  /* double the limit */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                     /* not part of the sequence (offset 1) */
        return (seq_t)1;

    /* Ensure we have a reasonable number of primes cached initially */
    expand_prime_cache(2 * n + 10);   /* a rough upper bound for the needed primes */

    seq_t result = (seq_t)1;          /* product accumulator */
    size_t coprime_count = 0;         /* how many coprime primes we have multiplied */
    size_t idx = 0;                   /* current index in the prime list */

    /* Iterate over primes until we have collected n coprime ones */
    while (coprime_count < (size_t)n) {
        /* If we reached the end of the cached list, grow it */
        if (idx >= prime_cnt) {
            expand_prime_cache(prime_cnt * 2 + 100);
            continue;
        }

        uint64_t p = prime_list[idx++];
        if (seq_gcd((seq_t)p, (seq_t)n) == (seq_t)1) {
            result *= (seq_t)p;
            ++coprime_count;
        }
    }

    return result;
}
