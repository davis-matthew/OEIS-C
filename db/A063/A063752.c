/*
 * A063752  Numbers k such that cototient(k) is a square.
 * https://oeis.org/A063752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 2205, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, factor_u64, floor_sqrt, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A063752
   Numbers k such that cototient(k) = k - phi(k) is a perfect square.
   a(n) = the n-th such integer (offset 1).
   The function computes terms on demand using a simple subsequence search
   with memoisation.  The search is efficient enough for n up to about
   10 000 (k < 2×10⁵). */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"
#include "../utils/ensure_capacity.h"

static seq_t *cache      = NULL;   /* stored terms */
static size_t  cache_len = 0;      /* number of stored terms   */
static size_t  cache_cap = 0;      /* allocated capacity       */
static idx_t   last_k    = 0;      /* last k that has been tested */

/* Test if x is a perfect square using the provided floor_sqrt and seq_mul. */
static inline bool is_square(seq_t x)
{
    seq_t sqr = floor_sqrt(x);
    sqr = seq_mul(sqr, sqr);
    return sqr == x;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Fill the cache until we have at least n terms. */
    while (cache_len < n) {
        last_k++;  /* next candidate integer k */

        /* compute phi(k) with uint64_t arithmetic */
        uint64_t k   = (uint64_t)last_k;
        uint64_t pr[64], ex[64];  /* enough for all occurrences up to 1e12 */
        size_t   cnt;
        factor_u64(k, pr, ex, &cnt);

        uint64_t phi = k;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = pr[i];
            phi = (phi / p) * (p - 1);
        }

        uint64_t cot = k - phi;          /* cototient = k - phi(k) */
        seq_t   cot_seq = (seq_t)cot;     /* cast to seq_t for sqrt */

        if (is_square(cot_seq)) {
            /* store the term into the cache */
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
            cache[cache_len++] = (seq_t)k;
        }
    }

    /* return the n-th term (1-based index) */
    return cache[n - 1];
}
