/*
 * A067783  Numbers k such that prime(k+1)^2 == prime(k)^2 (mod k).
 * https://oeis.org/A067783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k such that prime(k+1)^2 ≡ prime(k)^2 (mod k). The
 *            sequence is defined by scanning k = 1,2,3,… and collecting those
 *            k that satisfy the congruence. The function stores the
 *            qualifying k's in a static cache so that subsequently requested
 *            indices can be answered in O(1). This implementation wo...
 * Range:     u8 n <= 17, u16 n <= 42, u32 n <= 44, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */

/* Cache of found k values.  cache[i] is the (i+1)-th term. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Next k value to test.  We start at 1 (offset 1). */
static idx_t next_k = 1;

/* Ensure cache capacity for at least one more element. */
static void
ensure_cap(void)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                 /* allocation failure */
        cache      = tmp;
        cache_cap  = new_cap;
    }
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* below offset – undefined */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        idx_t k  = next_k;
        uint32_t pk  = get_prime(k);     /* prime(k)   */
        uint32_t pk1 = get_prime(k+1);   /* prime(k+1) */

        seq_t mod = (seq_t)k;

        /* Compute (prime(k)^2) mod k and (prime(k+1)^2) mod k. */
        seq_t pkm = (seq_t)(pk  % (uint64_t)k);
        seq_t pk1m= (seq_t)(pk1 % (uint64_t)k);
        seq_t pk_mod  = (pkm  * pkm)  % mod;
        seq_t pk1_mod = (pk1m * pk1m) % mod;

        if (pk_mod == pk1_mod) {
            ensure_cap();
            cache[cache_len++] = (seq_t)k;
        }

        ++next_k;
    }

    return cache[n - 1];
}
