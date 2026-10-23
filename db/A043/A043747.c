/*
 * A043747  Numbers n such that number of runs in the base 2 representation of
 *          n is congruent to 1 mod 8.
 * https://oeis.org/A043747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A043747: numbers n whose binary representation has
 * a number of runs ≡ 1 (mod 8).  The function rebuilds or extends
 * an internal cache so that any call a(n) returns the nth such number
 * (a(1) = 1).  It works for both C11 and C++17 with any seq_t
 * (built‑in integer type or an arbitrary‑precision class). */
#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
#include <cstddef>
#endif

/* Return the position of the most significant bit of x (x>0). */
static inline int msb_pos(uint64_t x)
{
    int pos = 0;
    while (x >>= 1) ++pos;
    return pos;
}

/* seq_t a(idx_t n) */
seq_t a(idx_t n)
{
    /* Static cache for previously found terms. */
    static seq_t *cache   = NULL;
    static size_t capacity = 0;
    static size_t length   = 0;          /* number of terms stored so far */
    static uint64_t last_candidate = 0;   /* last integer examined */

    /* If we already have enough terms, simply return the requested one. */
    while (length < n)
    {
        /* Next integer to test. */
        ++last_candidate;

        /* Compute the number of runs in its binary representation. */
        uint64_t x = last_candidate;
        int msb   = msb_pos(x);
        int prev  = (x >> msb) & 1;
        int runs  = 1;

        for (int i = msb - 1; i >= 0; --i)
        {
            int cur = (x >> i) & 1;
            if (cur != prev)
            {
                prev = cur;
                ++runs;
            }
        }

        /* If runs ≡ 1 (mod 8), store the candidate. */
        if ((runs & 7) == 1)
        {
            /* Grow the cache if necessary. */
            if (length == capacity)
            {
#ifdef __cplusplus
                /* C++: use new/delete to preserve arbitrary‑precision types. */
                size_t new_cap = capacity ? capacity * 2 : 64;
                seq_t *tmp = new seq_t[new_cap];
                for (size_t i = 0; i < length; ++i) tmp[i] = cache[i];
                delete[] cache;
                cache   = tmp;
                capacity = new_cap;
#else
                /* C: realloc works because seq_t is a trivial type. */
                size_t new_cap = capacity ? capacity * 2 : 64;
                seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { exit(EXIT_FAILURE); }
                cache   = tmp;
                capacity = new_cap;
#endif
            }
            cache[length++] = (seq_t)last_candidate;
        }
    }

    /* Return the cached term. */
    return cache[n - 1];
}
