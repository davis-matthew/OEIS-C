/*
 * A128955  Numbers n such that n+(n+1)^3 is prime.
 * https://oeis.org/A128955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer m such that m + (m+1)^3 is prime
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 13, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"

/* Return the nth term of A128955.  a(1) is the first term (2). */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of terms computed */
    static uint64_t last_checked = 0;/* last candidate tested */

    if (n == 0) return 0; /* undefined, but return 0 */

    /* compute terms until we have at least n of them */
    while (cache_len < n) {
        ++last_checked; /* next candidate m */

        /* compute m + (m+1)^3 using 128‑bit arithmetic */
        unsigned __int128 mp1 = (unsigned __int128)(last_checked + 1);
        unsigned __int128 cube = mp1 * mp1 * mp1;
        unsigned __int128 val128 = (unsigned __int128)last_checked + cube;
        seq_t val = (seq_t)val128;   /* cast to seq_t (fits 64‑bit here) */

        if (is_prime_big(val)) {
            /* ensure capacity for new term */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                while (new_cap <= cache_len) new_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp; cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
    return cache[n - 1];
}
