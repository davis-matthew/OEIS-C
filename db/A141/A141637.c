/*
 * A141637  Primes of the form n/(p(n+1)-p(n)), where p(n)=n-th prime.
 * https://oeis.org/A141637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime formed by n / (p(n+1) – p(n)), where p(k) is the
 *            k‑th prime. The function returns the n‑th element of OEIS
 *            A141637 (offset 1). The sequence is computed lazily and cached
 *            for subsequent calls
 * Range:     u8 n <= 25, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   build_primes_upto, is_prime, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/*  Utilities supplied by the build system  */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes, size_t *cnt); */
#include "../utils/is_prime.h"           /* bool is_prime(idx_t n); */
#include "../utils/seq_alloc_array.h"    /* seq_t *seq_alloc_array(size_t n); */

static const idx_t PRIME_LIMIT = 600000ULL;   /* Sufficient for n up to ~10 000 */

static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static idx_t    next_n        = 1;   /* next n to examine   */

/* Cache of found sequence terms */
static seq_t *term_cache   = NULL;
static size_t cache_len    = 0;
static size_t cache_cap    = 0;

/* Grow the term cache to at least min_cap elements */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_arr = seq_alloc_array(new_cap);
    if (!new_arr) return;                      /* allocation failed */
    for (size_t i = 0; i < cache_len; ++i) new_arr[i] = term_cache[i];
    free(term_cache);
    term_cache = new_arr;
    cache_cap  = new_cap;
}

/* Ensure the prime table is at least PRIME_LIMIT */
static void ensure_primes(void)
{
    if (!prime_list) {
        build_primes_upto(PRIME_LIMIT, &prime_list, &prime_count);
    }
}

seq_t a(idx_t n)
{
    if (n <= 0) return (seq_t)0;   /* undefined below offset */

    ensure_primes();

    /* Generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        if (next_n >= prime_count) {
            /* Out of primes – should not happen for the intended n range */
            return (seq_t)0;
        }

        uint64_t diff = prime_list[next_n] - prime_list[next_n - 1];
        if (diff != 0 && next_n % diff == 0) {
            idx_t val = next_n / diff;
            if (is_prime(val)) {
                grow_cache(cache_len + 1);
                term_cache[cache_len++] = (seq_t)val;
            }
        }
        next_n++;
    }

    return term_cache[n - 1];
}
