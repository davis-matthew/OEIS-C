/*
 * A078282  a(1) = 1, a(n) is the smallest multiple of n which begins with
 *          a(n-1) and is greater than a(n-1).
 * https://oeis.org/A078282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; a(n) = smallest multiple of n whose decimal representation
   starts with a(n-1) (for n > 1). */

#include <stdlib.h>
#include "../utils/mul_mod64.h"   /* for mul_mod64 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* index < 1 not used */

    /* static memoisation tables */
    static seq_t *cache = NULL;   /* cache[i] holds a(i), i >= 1           */
    static size_t cache_cap = 0;  /* allocated capacity (in elements)     */
    static size_t cache_len = 0;  /* largest index already computed       */

    /* Grow cache if needed */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 1;
        while ((size_t)n >= new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap + 1;            /* +1 to keep index 0 unused */
    }

    /* Compute all missing terms up to n */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
            continue;
        }

        seq_t prev = cache[i - 1];
        idx_t ni   = (idx_t)i;

        /* Current power of 10 used for digits, and its modulus modulo n */
        seq_t pow10   = (seq_t)10;
        idx_t pow10mod = 10 % ni;

        /* Search minimal d such that (prev*10^d + r) % n == 0 with r < 10^d */
        while (1) {
            idx_t prev_mod = (idx_t)(prev % (seq_t)ni);          /* prev % n */
            idx_t prod_mod = mul_mod64(prev_mod, pow10mod, ni); /* (prev_mod*10^d) % n */
            idx_t r = (prod_mod == 0) ? 0 : ni - prod_mod;

            if ((seq_t)r < pow10) {                    /* r is usable */
                cache[i] = prev * pow10 + (seq_t)r;
                break;
            }

            /* increase d */
            pow10   = pow10 * (seq_t)10;
            pow10mod = mul_mod64(pow10mod, 10, ni);
        }
    }

    cache_len = n;
    return cache[n];
}
