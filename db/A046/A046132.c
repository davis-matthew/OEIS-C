/*
 * A046132  Larger member p+4 of cousin primes (p, p+4).
 * https://oeis.org/A046132
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the larger member p+4 of the nth cousin prime pair (p,
 *            p+4), where a(1) = 7, a(2) = 11, … (OEIS A046132). The function
 *            keeps an internal cache of the terms that have been generated,
 *            so subsequent calls are fast and the order of the calls does not
 *            matter
 * Range:     u8 n <= 16, u16 n <= 860, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Simple primality test for 64‑bit unsigned integers (trial division). */
/* ------------------------------------------------------------------ */
static inline bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    if (n % 2 == 0)
        return n == 2;
    if (n % 3 == 0)
        return n == 3;

    /* check divisibility by numbers of the form 6k±1 up to sqrt(n) */
    for (uint64_t i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Sequence implementation                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)                     /* safety: OEIS offset is 1 */
        return 0;

    /* static cache of computed terms */
    static seq_t *cache     = NULL;
    static idx_t  cache_len = 0;
    static idx_t  cache_cap = 0;

    /* candidate for the smaller cousin prime (p); start at 3 */
    static idx_t candidate = 3;

    /* generate terms until cache_len >= n */
    while (cache_len < n)
    {
        /* Ensure candidate is odd */
        if ((candidate & 1) == 0)
            candidate++;

        /* Test if candidate and candidate+4 are both primes */
        if (is_prime_uint64(candidate) && is_prime_uint64(candidate + 4))
        {
            /* Extend cache if necessary */
            if (cache_len == cache_cap)
            {
                uint64_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                {
                    /* In a real program you might handle the error differently */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }

            /* Store the larger member of the cousin pair (p+4) */
            cache[cache_len++] = (seq_t)(candidate + 4);
        }

        candidate += 2;   /* move to the next odd number */
    }

    return cache[n - 1];
}
