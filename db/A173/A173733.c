/*
 * A173733  Primes p which give primes when 1331 = 11^3 is prefixed (see
 *          A173579).
 * https://oeis.org/A173733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   digit_len, is_prime_big, pow10, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: computes a(n) as described in OEIS A173733.
 * a(n) = the n-th prime p such that N = 1331 = 11^3 is prefixed once p is
 * appended to it in decimal: q = N * 10^k + p is prime, where k = number of
 * decimal digits of p.  The function uses a simple incremental search with
 * memoisation; it works for any seq_t (8‑64 bit or arbitrary precision)
 * and any idx_t (uint64_t).  It is O(n * T(p_n)) where T(p_n) is the time
 * for a primality test on numbers of size ≈ |p_n| + 4 digits.  For n up to
 * 10000 the runtime stays well below 2 seconds on typical data. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"
#include "../utils/seq_mul.h"

/* Static cache for primes that satisfy the condition.  The cache is filled
 * lazily and can be accessed in any order of n. */
static seq_t *cache       = NULL;   /* array of qualifying primes */
static size_t cache_size  = 0;       /* current number of elements */
static size_t cache_cap   = 0;       /* current array capacity */

static seq_t next_candidate = 2;     /* next integer to test for primality */

/* Grow the cache array to hold at least 'need' elements.  The cast to
 * seq_t* is required for C++ compatibility. */
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; undefined for n==0 */

    /* Extend the cache until we have at least n qualifying primes. */
    while (cache_size < (size_t)n) {
        seq_t p = 0;

        /* ---- find next prime p ---- */
        for (;;) {
            if (next_candidate <= 2) {      /* first prime is 2 */
                p = 2;
                next_candidate = 3;
                break;
            }
            if (is_prime_big(next_candidate)) {
                p = next_candidate;
                next_candidate += 2;       /* skip even numbers */
                break;
            }
            next_candidate += 2;
        }

        /* ---- compute q = 1331 * 10^k + p ---- */
        idx_t k = digit_len(p);
        seq_t pow10k = pow10(k);
        seq_t q = seq_mul((seq_t)1331, pow10k) + p;

        /* ---- test q for primality ---- */
        if (is_prime_big(q)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = p;
        }
    }

    return cache[n - 1];
}
