/*
 * A181555  a(n) = A002110(n)^n.
 * https://oeis.org/A181555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002110(n)^n (primorial(n)^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   _prime_limit_for_idx, big_pow, build_primes_upto, seq_mul,
 *            seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* malloc, realloc, free */
#include <stdint.h>          /* uint64_t */
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/seq_one.h"
#include "../utils/seq_mul.h"
#include "../utils/big_pow.h"

/* Cached list of primes (uint64_t) */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;

/* Cached primorials: prim_cache[i] = product of first i primes (prim_cache[0] = 1) */
static seq_t   *prim_cache    = NULL;
static size_t   prim_cap      = 0;   /* allocated capacity */
static size_t   prim_pos      = 0;   /* next index to compute */

/* Ensure at least n primes are available in prime_list. */
static void ensure_primes(idx_t n)
{
    if (prime_count >= (size_t)n)
        return;                       /* already enough */

    /* free old list if present */
    if (prime_list != NULL) {
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
    }

    /* upper bound for the nth prime */
    idx_t limit = _prime_limit_for_idx(n);

    /* build primes up to the limit */
    build_primes_upto(limit, &prime_list, &prime_count);
}

/* Ensure prim_cache holds primorials up to n. */
static void ensure_prim_cache(idx_t n)
{
    /* allocate cache if not yet present */
    if (prim_cache == NULL) {
        prim_cap   = (size_t)n + 1;
        prim_cache = (seq_t *)malloc(prim_cap * sizeof(seq_t));
        prim_cache[0] = seq_one();
        prim_pos   = 1;
    }

    /* grow capacity if needed */
    if (prim_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        prim_cache = (seq_t *)realloc(prim_cache, new_cap * sizeof(seq_t));
        prim_cap   = new_cap;
    }

    /* compute primorials up to n */
    while (prim_pos <= (size_t)n) {
        seq_t prev = prim_cache[prim_pos - 1];
        seq_t p    = (seq_t)prime_list[prim_pos - 1]; /* cast prime to seq_t */
        prim_cache[prim_pos] = seq_mul(prev, p);
        prim_pos++;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();            /* A002110(0)=1, 1^0 = 1 */

    ensure_primes(n);
    ensure_prim_cache(n);

    return big_pow(prim_cache[(size_t)n], (seq_t)n);
}
