/*
 * A153276  Numbers n >= 0 such that 5*n+6 is not prime.
 * https://oeis.org/A153276
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th non‑negative integer n such that 5*n+6 is
   NOT prime.  (offset 1 – a(1)=0, a(2)=2, …)  The function performs a
   search by testing each candidate and caches the results so that calls
   in any order are fast. */

#include <stddef.h>                 /* size_t */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t) */
#include "../utils/grow_seq_array.h"/* void grow_seq_array(seq_t **, size_t *, size_t) */

seq_t a(idx_t n)
{
    /* Handle the trivial pre‑offset case – callers should never use n < 1,
       but we return 0 to avoid undefined behaviour. */
    if (n < 1) return (seq_t)0;

    static seq_t *cache      = NULL;   /* stored n values */
    static size_t cache_len = 0;       /* number of stored values */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t next_cand   = 0;      /* next n to test */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        /* 5*n + 6 in the type of the sequence. */
        seq_t val = ((seq_t)5) * (seq_t)next_cand + (seq_t)6;

        /* Store n if 5*n+6 is composite. */
        if (!is_prime_big(val)) {
            /* Grow the cache if needed. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                grow_seq_array(&cache, &cache_cap, new_cap);
            }
            cache[cache_len++] = (seq_t)next_cand;
        }
        ++next_cand;
    }

    return cache[n - 1];
}
