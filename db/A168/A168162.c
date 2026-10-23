/*
 * A168162  Numbers n which do not exceed the sum of the binary digits in all
 *          primes <= n.
 * https://oeis.org/A168162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A168162: return the nth number n such that
   n <= sum of popcount(p) for all primes p <= n.
   The sequence is indexed from 1 (a(1)=3).  Since the definition
   is based on a search, we build the values on demand via a
   simple sieve of Eratosthenes. */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* static cache for computed terms */
static seq_t *seq_arr = NULL;   /* array of sequence terms */
static size_t seq_cap = 0;     /* allocated size of seq_arr   */
static idx_t  seq_len = 0;     /* number of terms stored     */

/* current sieve limit that has been processed */
static idx_t sieve_limit = 0;

/* rebuild the sequence up to numeric limit 'limit' */
/* This function discards any previously cached values. */
static void recompute_sequence(idx_t limit)
{
    bool *is_prime = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!is_prime) abort();
    memset(is_prime, true, (size_t)(limit + 1));
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* free previous cache */
    free(seq_arr);
    seq_arr = NULL;
    seq_cap = 0;
    seq_len = 0;

    seq_t sum = 0;
    for (idx_t v = 2; v <= limit; ++v) {
        if (is_prime[v]) {
            sum += (seq_t)__builtin_popcountll((unsigned long long)v);
        }
        if (sum >= (seq_t)v) {
            if (seq_len == seq_cap) {
                seq_cap = seq_cap * 2 + 10;
                seq_arr = (seq_t *)realloc(seq_arr, seq_cap * sizeof(seq_t));
                if (!seq_arr) abort();
            }
            seq_arr[seq_len++] = (seq_t)v;
        }
    }

    free(is_prime);
}

/* ensure that the cache contains at least 'target' terms */
static void ensure_sequence(idx_t target)
{
    if (seq_len >= target) return;

    /* start with a modest limit and double it until enough terms */
    idx_t limit = sieve_limit ? sieve_limit * 2 : 1000;
    while (true) {
        recompute_sequence(limit);
        if (seq_len >= target) break;
        sieve_limit = limit;
        limit *= 2;
    }
    sieve_limit = limit;
}

/* public function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not defined for 0, but guard against */
    ensure_sequence(n);
    return seq_arr[n - 1];
}
