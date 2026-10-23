/*
 * A117512  Triangular numbers for which the sum of the digits is a prime
 *          number.
 * https://oeis.org/A117512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   digit_sum, grow_seq, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate triangular numbers T_k = k*(k+1)/2 and keep those
   whose digit sum is prime.  The k‑th such triangular number is the
   n‑th term of A117512. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_small.h"
#include "../utils/grow_seq.h"

static seq_t *cache = NULL;    /* cached triangular numbers */
static size_t cache_len = 0;   /* number of cached elements     */
static size_t cache_cap = 0;   /* allocated capacity            */
static idx_t  last_k   = 0;    /* last k whose T_k was examined */

/* Ensure the cache can hold at least 'min_cap' elements.  Uses the
   provided grow_seq helper so that seq_t objects are constructed
   correctly in both C and C++. */
static void ensure_cache_cap(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    grow_seq(&cache, &cache_cap, min_cap);
}

/* Return the n-th term of A117512 (triangular numbers whose digit
   sum is prime).  The function is memoised; calls need not be in
   increasing order. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for offset 1 */

    /* Expand cache if necessary. */
    ensure_cache_cap(n);

    /* Generate new triangular numbers until we have n terms. */
    while (cache_len < (size_t)n) {
        idx_t        k   = last_k + 1;   /* next candidate index   */
        seq_t        tri = ((seq_t)k) * ((seq_t)(k + 1));
        tri /= 2;                       /* T_k = k*(k+1)/2        */
        seq_t        ds  = digit_sum(tri);   /* digit sum of T_k   */

        if (is_prime_small(ds))
            cache[cache_len++] = tri;   /* store if digit sum is prime */

        last_k = k;
    }

    return cache[n - 1];
}
