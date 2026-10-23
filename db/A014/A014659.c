/*
 * A014659  Odd numbers that do not divide 2^k + 1 for any k >= 1.
 * https://oeis.org/A014659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) is the n‑th odd integer N>1 for which
   no exponent k ≥ 1 satisfies 2^k ≡ –1 (mod N). */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* Helper: test whether an odd m > 1 divides 2^k + 1 for some k ≥ 1.
   Returns true if such k exists (i.e., 2^k ≡ –1 mod m). */
static bool _has_minus_one_power(idx_t m)
{
    idx_t val    = 2 % m;          /* 2^1 mod m */
    idx_t target = m - 1;          /* –1 mod m */

    while (val != target && val != 1)
        val = mod_mul((uint64_t)val, 2ULL, (uint64_t)m);  /* val = 2*val mod m */

    return val == target;
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t cache_len  = 0;
    static idx_t next_candidate = 3;   /* first odd integer > 1 */

    /* generate terms until we have at least n */
    while (cache_len < n)
    {
        /* make sure we have space */
        if (cache_len == cache_cap)
        {
            if (cache_cap == 0) cache_cap = 64;
            else                cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* test next odd integer */
        idx_t m = next_candidate;
        next_candidate += 2;               /* advance for next call */

        if (!_has_minus_one_power(m))
        {
            cache[cache_len++] = (seq_t)m;   /* store the term */
        }
    }

    /* cache[0] holds a(1), so return cache[n-1] */
    return cache[n - 1];
}
