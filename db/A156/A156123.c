/*
 * A156123  Primes p such that p+36 and p+144 are both prime.
 * https://oeis.org/A156123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 255, u32 n <= 473, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for primes p such that p+36 and p+144 are also prime */
/* This implementation keeps a growing cache of all such primes found so far.
   It builds an increasing list of ordinary primes by repeated sieving
   (using build_primes_upto) and tests each prime for the required
   primality conditions with is_prime_big.  The sequence is cached in a
   static array and the cache is extended only if further terms are needed.
   The function works for any integer type seq_t supplied by the build
   and is O(n) with a small constant factor.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static seq_t *seq = NULL;          /* cache of sequence terms               */
static size_t seq_len = 0;         /* number of terms currently cached      */
static size_t seq_cap = 0;         /* current capacity of the cache         */
static idx_t   current_limit = 0;  /* highest value sieved up to so far     */

/* Helper that sieves all primes <= limit and appends those primes p
   with p+36 and p+144 prime to the global cache.  The cache contents
   are not cleared; duplicates are avoided by checking the last
   inserted term. */
static void extend_cache(idx_t limit)
{
    uint64_t *prime_arr = NULL;
    size_t    prime_cnt = 0;
    build_primes_upto(limit, &prime_arr, &prime_cnt);

    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)prime_arr[i];

        /* p+144 must still lie within the sieved range to ensure
           we can test it with is_prime_big efficiently. */
        if (p + (seq_t)144u > (seq_t)limit) break;

        seq_t p36  = p + (seq_t)36u;
        seq_t p144 = p + (seq_t)144u;
        if (is_prime_big(p36) && is_prime_big(p144)) {
            if (seq_len == 0 || seq[seq_len - 1] != p) {
                grow_seq(&seq, &seq_cap, seq_len + 1);
                seq[seq_len++] = p;
            }
        }
    }
    free(prime_arr);
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence is 1‑based only */

    /* Ensure the cache is large enough to hold the first n terms. */
    while (seq_len < n) {
        /* Start sieving at a modest limit and double it each
           iteration until enough terms are found. */
        if (current_limit == 0) current_limit = 1000000u;   /* 1 M */
        else                      current_limit *= 2;

        extend_cache(current_limit);
    }

    return seq[n - 1];              /* 0‑based indexing internally */
}
