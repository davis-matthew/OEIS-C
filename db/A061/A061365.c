/*
 * A061365  Numbers k such that either k or its reversal (but not both) is
 *          prime.
 * https://oeis.org/A061365
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A061365: Numbers k such that exactly one of k or rev(k) is prime.
    The routine is memoised; each evaluation may finish in O(1) amortised
    time after the first call.                                     */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"
#include "../utils/reverse_decimal.h"

static seq_t *cache    = NULL;   /* buffer of found terms                */
static idx_t  cache_sz = 0;      /* number of stored terms              */
static idx_t  cache_cap = 0;     /* current capacity of cache           */
static seq_t  next_candidate = 1;/* next integer to examine             */

/* Ensure cache has room for at least 'need' elements.              */
static void grow_cache(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { abort(); } /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------
 *  Return the nth term of A061365 (offset = 1).  The function works
 *  correctly for any n >= 1, in any call order.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defensive guard – offset is 1 */

    while (cache_sz < n) {
        bool pk = is_prime_big(next_candidate);
        seq_t rev = reverse_decimal(next_candidate);
        bool pr = is_prime_big(rev);
        if (pk != pr) {              /* exactly one of them is prime */
            grow_cache(cache_sz + 1);
            cache[cache_sz++] = next_candidate;
        }
        ++next_candidate;            /* proceed to the next integer   */
    }
    return cache[n - 1];
}
