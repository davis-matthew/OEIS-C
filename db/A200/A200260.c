/*
 * A200260  Numbers k such that k-th prime has an odd digit sum.
 * https://oeis.org/A200260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 3565, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, get_prime, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the k‑th prime has an odd digit sum. */
#include <stddef.h>
#include "../utils/get_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

static seq_t *cache          = NULL;   /* cache of a(1)..a(n) */
static size_t cache_cap      = 0;      /* allocated cache size  */
static size_t cache_terms    = 0;      /* number of terms stored  */
static idx_t  next_prime_idx = 1;      /* next prime index to test */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1 */

    if (cache_terms < (size_t)n) {
        /* Ensure enough space for results */
        if (cache_cap < (size_t)n) {
            cache = seq_realloc(cache, cache_cap, n);
            cache_cap = n;
        }
        /* Generate missing terms */
        while (cache_terms < (size_t)n) {
            uint32_t p = get_prime(next_prime_idx);          /* k‑th prime */
            seq_t   sum = digit_sum((seq_t)p);               /* digit sum of p */
            if ((sum & 1) != 0) {                           /* odd ? */
                cache[cache_terms] = (seq_t)next_prime_idx;  /* store k */
                cache_terms++;
            }
            next_prime_idx++;
        }
    }

    return cache[n-1];
}
