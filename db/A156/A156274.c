/*
 * A156274  List of prime pairs of the form (p, p+6).
 * https://oeis.org/A156274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156274: interleaved primes p, p+6 where both are prime;
 * a(n) is the n‑th term of the list 5, 11, 7, 13, … */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1 – behaviour for n<1 is irrelevant */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_p = 5;       /* first prime that can start a pair */

    while (cache_len < n) {
        /* test whether both next_p and next_p+6 are prime */
        if (is_prime(next_p) && is_prime(next_p + 6)) {
            /* ensure capacity for two new terms */
            if (cache_len + 2 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp;
#ifdef __cplusplus
                tmp = new seq_t[new_cap];
                for (size_t i = 0; i < cache_len; ++i)
                    tmp[i] = cache[i];
                delete[] cache;
#else
                tmp = realloc(cache, new_cap * sizeof(seq_t));
#endif
                if (!tmp) abort();   /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_p;
            cache[cache_len++] = (seq_t)(next_p + 6);
        }
        next_p += 2; /* next odd candidate */
    }

    return cache[n - 1]; /* zero‑based index into the cached terms */
}
