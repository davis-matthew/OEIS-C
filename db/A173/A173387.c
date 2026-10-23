/*
 * A173387  The absolute value of n-th prime number minus n-th number k such
 *          that 6*k-+1 is a twin prime pair.
 * https://oeis.org/A173387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = abs(A000040(n) - A002822(n))
 * Range:     u8 n <= 49, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the A173387 sequence as defined in OEIS. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

/* --------------------------------------------------------------------
 * Cache of k-values from twin‑prime pairs: k such that 6k-1 and 6k+1 are
 * both prime.  k_cache[i] holds the (i+1)-th such k.
 * -------------------------------------------------------------------- */
static idx_t *k_cache = NULL;   /* pointer to array of k values          */
static size_t  k_cap   = 0;      /* current allocation capacity (in elements) */
static size_t  k_cnt   = 0;      /* number of k-values cached so far     */
static idx_t  next_k    = 1;     /* next candidate k to test             */

/* Ensure that the cache contains at least n k-values (1‑based).  */
static void
ensure_k_cache(idx_t n)
{
    if (n <= k_cnt)            /* already sufficient                      */
        return;

    /* Allocate the array if needed.  The cast is required for C++.   */
    if (!k_cache) {
        k_cap   = 16;
        k_cache = (idx_t *)malloc(k_cap * sizeof(idx_t));
    }

    while (k_cnt < n) {
        /* Test the current candidate k. */
        idx_t candidate = next_k;
        idx_t m1 = 6 * candidate - 1;
        idx_t m2 = 6 * candidate + 1;
        if (is_prime(m1) && is_prime(m2)) {
            /* Grow the array if it is currently full.          */
            if (k_cnt == k_cap) {
                k_cap *= 2;
                k_cache = (idx_t *)realloc(k_cache, k_cap * sizeof(idx_t));
            }
            k_cache[k_cnt++] = candidate;   /* store found k         */
        }
        ++next_k;  /* advance to next candidate                  */
    }
}

/* --------------------------------------------------------------------
 * Main sequence function.
 * -------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)   /* OEIS offset is 1; return 0 for n == 0 (unused). */
        return 0;

    /* Make sure the k-cache knows the n-th k.                       */
    ensure_k_cache(n);

    /* nth prime (1‑based).                                            */
    seq_t prime = (seq_t)get_prime(n);      /* get_prime returns uint64_t */

    /* nth k value from twin‑prime pair.                              */
    seq_t k = (seq_t)k_cache[n - 1];         /* convert to seq_t for subtraction */

    /* Compute absolute difference.                                    */
    return (prime > k) ? (prime - k) : (k - prime);
}
