/*
 * A176687  Numbers k such that k^2-1 is the product of 4 distinct primes.
 * https://oeis.org/A176687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 7325, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search for the smallest integer k such that
   k^2-1 is a product of 4 distinct primes.
   A(k) is the k-th such integer (OEIS offset 1). */

//  Helper libraries
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"   // factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)

//  --------------------------------------------------------------------
//  Cache of already found terms.  The array is resized as needed.
//  --------------------------------------------------------------------
static seq_t *cache       = NULL;
static size_t cache_len   = 0;   // number of cached terms
static size_t cache_cap   = 0;   // allocated capacity
static idx_t  last_k      = 2;   // last even k examined

static void push_term(seq_t k)
{
    if (cache == NULL) {
        cache   = (seq_t *)malloc(32 * sizeof(seq_t));
        cache_cap = 32;
    } else if (cache_len >= cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = k;
}

//  --------------------------------------------------------------------
//  Test whether an even integer k satisfies "k^2-1 = p1*p2*p3*p4"
//  with distinct primes.  This is done by factoring (k-1) and (k+1)
//  separately with factor_u64 and checking for square‑freeness
//  and a total of exactly four distinct prime factors.
//  --------------------------------------------------------------------
static bool good_k(seq_t k)
{
    if ((idx_t)k & 1) return false;               // we search only even k

    uint64_t km1 = (uint64_t)k - 1;
    uint64_t kp1 = (uint64_t)k + 1;

    uint64_t pr1[32], ex1[32];
    size_t  n1 = 0;
    factor_u64(km1, pr1, ex1, &n1);

    uint64_t pr2[32], ex2[32];
    size_t  n2 = 0;
    factor_u64(kp1, pr2, ex2, &n2);

    if (n1 + n2 == 0) return false;  // should not happen

    //   Ensure all exponents are 1 (square‑free) and collect unique primes
    uint64_t uniq[8];   // at most 4 primes in total
    size_t   uniq_cnt = 0;

    for (size_t i = 0; i < n1; ++i) {
        if (ex1[i] != 1) return false;
        uniq[uniq_cnt++] = pr1[i];
    }

    for (size_t i = 0; i < n2; ++i) {
        if (ex2[i] != 1) return false;
        // check for duplicates
        for (size_t j = 0; j < uniq_cnt; ++j) {
            if (uniq[j] == pr2[i]) return false;     // repeated prime
        }
        uniq[uniq_cnt++] = pr2[i];
    }

    return uniq_cnt == 4;
}

//  --------------------------------------------------------------------
//  Main function required by the build.  It guarantees that a(n)
//  returns the n-th member of the sequence, for every n ≥ 1.
//  --------------------------------------------------------------------
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   // undefined before the offset

    //  generate terms until we have at least n of them
    while (cache_len < n) {

        // move to the next even integer
        if (last_k & 1) ++last_k;
        if (!good_k((seq_t)last_k)) {
            last_k += 2;
            continue;
        }

        push_term((seq_t)last_k);
        last_k += 2;   // advance to next candidate
    }

    return cache[n - 1];
}
