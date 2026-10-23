/*
 * A102386  Numbers k such that k99999 is prime.
 * https://oeis.org/A102386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that k99999 is prime
 * Implementation: iterate k = 1,2,3,…, test primality of N = 100000*k + 99999
 * with is_prime_uint64; cache every successful k.  The cache persists between
 * calls, so subsequent queries of any order are O(1) after the needed terms
 * have been generated.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

static seq_t *terms = NULL;     /* cached k values            */
static idx_t  size  = 0;       /* number of cached terms     */
static idx_t  cap   = 0;       /* capacity of the cache      */
static uint64_t next_k = 1;    /* next k to test             */

static void ensure_terms(idx_t need)
{
    if (cap >= need)
        return;
    size_t new_cap = cap ? cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined; guard against bad call */

    /* Generate terms until we have at least n of them */
    while (size < n) {
        ensure_terms(size + 1);
        for (;;) {
            uint64_t N = next_k * 100000ULL + 99999ULL;
            if (is_prime_uint64(N)) {
                terms[size++] = (seq_t)next_k;
                next_k++;   /* move past the found k */
                break;
            }
            next_k++;
        }
    }
    return terms[n - 1];
}
