/*
 * A108902  Numbers k such that 23*k + 2357 is prime.
 * https://oeis.org/A108902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A108902: numbers k such that 23*k + 2357 is prime. 
   The function returns the n-th such k (a(1) = 0). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Persistent storage of found k's (using uint64_t as intermediate). */
static idx_t *k_terms = NULL;   /* array of generated k values */
static idx_t  k_size   = 0;     /* number of terms stored so far */
static idx_t  k_cap    = 0;     /* allocated capacity of k_terms */
static idx_t  next_k   = 0;     /* next k to test */

/* Grow the k_terms array if needed. */
static void ensure_k_cap(idx_t needed)
{
    if (k_cap < needed) {
        idx_t newcap = k_cap ? k_cap << 1 : 16;
        if (newcap < needed) newcap = needed;
        k_terms = (idx_t *)realloc(k_terms, newcap * sizeof(idx_t));
        k_cap = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* offset starts at 1, return 0 for safety */

    /* Generate terms until we have at least n values. */
    while (k_size < n) {
        uint64_t val = 23ULL * next_k + 2357ULL; /* 23*k + 2357 */
        if (is_prime(val)) {
            ensure_k_cap(k_size + 1);
            k_terms[k_size++] = next_k;
        }
        next_k++;
    }

    return (seq_t)k_terms[n - 1];
}
