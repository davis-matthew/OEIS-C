/*
 * A132656  Motzkin number M(n) raised to power M(n).
 * https://oeis.org/A132656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = M(n)^M(n) where M(n) is the Motzkin number. M(0)=1,
 *            M(1)=1, M(n)=((2n+1)M(n-1)+3(n-1)M(n-2))/(n+2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 6 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* Static cache for Motzkin numbers. */
    static seq_t *motz = NULL;
    static size_t cap = 0;
    static idx_t last = 0;          /* largest index computed so far */

    if (n == 0 || n == 1) {
        /* Motzkin numbers for 0 and 1 are 1. */
        static seq_t one = (seq_t)1;
        return one;
    }

    /* Ensure the cache is large enough. */
    ensure_cache(n, &motz, &cap);

    /* Initialize the first two terms if not already done. */
    if (last < 1) {
        motz[0] = (seq_t)1;
        motz[1] = (seq_t)1;
        last = 1;
    }

    /* Compute Motzkin numbers up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t m_prev2 = motz[i - 2];
        seq_t m_prev  = motz[i - 1];

        /* Numerator: (2i+1)*M(i-1) + 3(i-1)*M(i-2) */
        seq_t num1 = (((seq_t)2 * (seq_t)i) + (seq_t)1) * m_prev;
        seq_t num2 = ((seq_t)3 * ((seq_t)i - (seq_t)1)) * m_prev2;
        seq_t sum  = num1 + num2;

        /* Denominator: i+2 */
        seq_t denom = (seq_t)(i + 2);

        /* Division yields the Motzkin number. */
        seq_t m_curr = sum / denom;
        motz[i] = m_curr;
    }
    last = n;

    seq_t M_n = motz[n];      /* Motzkin number at index n */
    seq_t result = ipow(M_n, M_n); /* compute M(n)^M(n) */

    return result;
}
