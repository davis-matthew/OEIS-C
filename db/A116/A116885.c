/*
 * A116885  Integers k such that 0<|k^5-m^2|<= k for some integer m.
 * https://oeis.org/A116885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k > 0 such that there exists an *
 *            integer m with 0 < |k^5 - m^2| <= k. The function computes the *
 *            sequence by exhaustive search, caching found values
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Cached sequence values */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static idx_t next_k = 1;   /* next k to test */

    /* Allocate or grow cache if necessary */
    if (len < n)
    {
        if (!cache)
        {
            cap = 16;
            cache = (seq_t *)malloc(cap * sizeof(seq_t));
            if (!cache) abort();
        }

        /* Search for next k's until we have n terms */
        while (len < n)
        {
            seq_t k  = (seq_t)next_k;

            /* Compute k^5 using 128‑bit arithmetic to avoid overflow in
             * intermediate steps.  The result is truncated back to seq_t,
             * which is fine because we only need correct values that fit
             * in seq_t. */
            unsigned __int128 k128  = (unsigned __int128)k;
            unsigned __int128 k5_128 = k128 * k128 * k128 * k128 * k128;
            seq_t k5 = (seq_t)k5_128;

            /* Candidate m's are around sqrt(k^5).  We examine
             * floor(sqrt(k^5))−1, floor(sqrt(k^5)), and floor(sqrt(k^5))+1.
             */
            seq_t m0 = floor_sqrt(k5);
            bool ok = false;

            /* Helper lambda to test one candidate m */
            #define TEST_M(m)                                         \
            do {                                                     \
                unsigned __int128 mp  = (unsigned __int128)(m);      \
                unsigned __int128 mp2 = mp * mp;                    \
                if (mp2 != k5_128) {                                \
                    unsigned __int128 diff = mp2 > k5_128            \
                        ? mp2 - k5_128 : k5_128 - mp2;               \
                    if (diff <= (unsigned __int128)k)               \
                        ok = true;                                  \
                }                                                    \
            } while (0)

            if (m0 > 0)  TEST_M(m0 - 1);
            if (!ok)     TEST_M(m0);
            if (!ok)     TEST_M(m0 + 1);

            #undef TEST_M

            if (ok)
            {
                if (len >= cap)
                {
                    size_t new_cap = cap << 1;
                    if (new_cap == 0) new_cap = 16;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) abort();
                    cache = tmp;
                    cap = new_cap;
                }
                cache[len++] = k;
            }

            ++next_k;
        }
    }

    return cache[n - 1];
}
