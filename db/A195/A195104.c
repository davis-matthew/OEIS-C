/*
 * A195104  Smallest prime p such that p*t(n) +- 1 is a twin prime pair, where
 *          t(n)=A014574(n) is the n-th twin prime average.
 * https://oeis.org/A195104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A195104
   a(n) = smallest prime p such that p * t(n) ± 1 are both prime,
   where t(n) is the nth twin prime average (A014574).
   The function generates twin averages on demand and then searches
   for the smallest prime p satisfying the condition.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"       /* bool is_prime(idx_t)      */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t)  */

/* --- cache for twin prime averages (A014574) -------------------------------- */

static seq_t *twin_cache = NULL;          /* array of averages          */
static size_t twin_cap   = 0;              /* allocated capacity         */
static idx_t  twin_cnt   = 0;              /* stored averages count      */
static idx_t  next_prime_candidate = 3;   /* next integer to test       */

/* Ensure twin_cache contains at least n entries (n >= 1). */
static void ensure_twin(idx_t n)
{
    if (twin_cap == 0) {
        twin_cap = 64;
        twin_cache = (seq_t *)malloc(twin_cap * sizeof(seq_t));
        if (!twin_cache) abort();
    }

    while (twin_cnt < n) {
        /* Find next prime */
        while (!is_prime(next_prime_candidate))
            next_prime_candidate++;

        /* Check for twin prime */
        if (is_prime(next_prime_candidate + 2)) {
            if (twin_cap <= twin_cnt) {                /* grow cache */
                twin_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(twin_cache, twin_cap * sizeof(seq_t));
                if (!tmp) abort();
                twin_cache = tmp;
            }
            twin_cache[twin_cnt++] = (seq_t)(next_prime_candidate + 1); /* average */
        }
        next_prime_candidate++;                         /* advance past this prime */
    }
}

/* --- main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset is 1; undefined for n==0 */

    /* Ensure we have the required twin average */
    ensure_twin(n);
    seq_t t = twin_cache[n - 1];        /* t(n) = A014574(n) */

    /* Search for the smallest prime p such that p*t ± 1 are both prime */
    idx_t p = 2;
    for (;;) {
        if (is_prime(p)) {
            seq_t m = (seq_t)p * t;
            if (is_prime_big(m - 1) && is_prime_big(m + 1))
                return (seq_t)p;
        }
        p++;
    }
}
