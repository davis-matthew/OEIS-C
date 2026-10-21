/*
 * A096784  Numbers k such that both k and k+1 are composite numbers that sum
 *          up to a prime.
 * https://oeis.org/A096784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((prime of the form 2k+1 with k and k+1 composite) - 1) /
 *            2. The function computes a(n) by searching for such k
 * Range:     u8 n <= 66, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrime.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_k = 1;  /* next candidate k to test */

/* Ensure the cache can hold 'needed' elements. */
static void ensure_cache_cap(size_t needed) {
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 4;
    if (needed > newcap) newcap = needed;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

/* Test whether n (>1) is composite. */
static inline bool isComposite(idx_t n) {
    return n > 1 && !isPrime(n);
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* A valid index starts from 1.  For n==0 we return 0 (undefined). */
    if (n == 0) return 0;

    /* Generate terms until we have the n-th. */
    while (cache_len < n) {
        /* Find the next k such that k and k+1 are composite and 2k+1 is prime. */
        while (true) {
            /* Move to next candidate. */
            ++next_k;
            if (isComposite(next_k) && isComposite(next_k + 1) && isPrime(2 * next_k + 1))
                break;
        }
        /* Store the resulting k. */
        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = (seq_t)next_k;
        ++cache_len;
    }
    return cache[n - 1];
}
