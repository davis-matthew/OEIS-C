/*
 * A235050  Squarefree numbers such that none of their prime factors share
 *          common 1-bits in the same bit-position of their binary
 *          representations.
 * https://oeis.org/A235050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the nth squarefree number whose prime factors have no
 *            common 1‑bits in their binary representations. Computation is
 *            done by scanning the integers in order and testing the required
 *            condition with a dynamic sieve of Eratosthenes
 * Range:     u8 n <= 69, u16 n <= 8289, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static idx_t sieve_limit = 0;          /* current upper bound of sieve */
static uint8_t *prime_flag = NULL;     /* prime_flag[i] == 1 iff i is prime */

/* Build a sieve of Eratosthenes up to 'limit' */
static void build_sieve(idx_t limit)
{
    free(prime_flag);
    prime_flag = (uint8_t *)malloc((size_t)(limit + 1));
    if (!prime_flag) exit(EXIT_FAILURE);

    memset(prime_flag, 1, (size_t)(limit + 1));
    prime_flag[0] = 0;
    if (limit >= 1) prime_flag[1] = 0;

    for (idx_t i = 2; (unsigned __int128)i * i <= limit; ++i) {
        if (prime_flag[i]) {
            idx_t start = (idx_t)((unsigned __int128)i * i);
            for (idx_t j = start; j <= limit; j += i)
                prime_flag[j] = 0;
        }
    }
    sieve_limit = limit;
}

/* Grow the sieve so that it covers at least 'min_limit' */
static void extend_sieve_to(idx_t min_limit)
{
    if (min_limit <= sieve_limit) return;
    idx_t new_limit = sieve_limit ? sieve_limit : 2;
    while (new_limit < min_limit) new_limit <<= 1;
    build_sieve(new_limit);
}

/* Sequence cache */
static seq_t *seq_cache    = NULL;
static size_t seq_cached  = 0;
static size_t seq_cache_cap = 0;
static idx_t last_checked     = 0;

/* Ensure the cache contains at least 'needed' terms */
static void ensure_cache_capacity(size_t needed)
{
    while (seq_cached < needed) {
        if (seq_cached >= seq_cache_cap) {
            seq_cache_cap = seq_cache_cap ? seq_cache_cap * 2 : 128;
            seq_t *tmp = (seq_t *)realloc(seq_cache, seq_cache_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            seq_cache = tmp;
        }

        ++last_checked;
        if (last_checked > sieve_limit) extend_sieve_to(last_checked * 2);

        bool ok = false;
        if (last_checked == 1) {
            ok = true;
        } else if (prime_flag[last_checked]) {
            ok = true;
        } else if ((last_checked & 1ULL) == 0) {           /* even composite */
            idx_t p = last_checked >> 1;
            if (p <= sieve_limit && prime_flag[p] && (p & 2ULL) == 0ULL)
                ok = true;
        }

        if (ok)
            seq_cache[seq_cached++] = (seq_t)last_checked;
    }
}

/* Public API */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but defined to 0 */

    if (sieve_limit == 0) build_sieve(1000);   /* initial modest bound */

    ensure_cache_capacity((size_t)n);
    return seq_cache[n - 1];
}
