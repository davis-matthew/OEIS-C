/*
 * A043701  Numbers whose base-12 representation has an even number of runs.
 * https://oeis.org/A043701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer whose base‑12 representation has an even number of runs. */

#include <stdlib.h>
#include <stddef.h>

/* Simple allocator that works both in C and in C++.
   In C++ it uses new[], in C it uses malloc. */
static seq_t* seq_alloc(size_t n)
{
#ifdef __cplusplus
    return new seq_t[n];
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

static void seq_free(seq_t* p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

/* Return the n‑th term of OEIS A043701. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;           /* array of found terms           */
    static idx_t cache_len = 0;           /* current number of stored terms */
    static idx_t cache_cap = 0;           /* allocated capacity              */
    static idx_t cur = 12;                /* next candidate to test         */

    while (cache_len < n) {
        idx_t x = cur;
        int prev_digit = -1;
        int runs = 0;

        /* Count runs in the base‑12 representation of cur. */
        while (x > 0) {
            int d = (int)(x % 12);
            if (d != prev_digit) {
                runs++;
                prev_digit = d;
            }
            x /= 12;
        }

        /* Accept the number if it has an even number of runs. */
        if ((runs & 1) == 0) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *new_cache = seq_alloc(new_cap);
                for (size_t i = 0; i < cache_len; ++i)
                    new_cache[i] = cache[i];
                seq_free(cache);
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur;
        }

        ++cur;
    }

    return cache[n - 1];
}
