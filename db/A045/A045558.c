/*
 * A045558  Numbers whose factorial has '36' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 3269, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A045558: numbers whose factorial has '36' as its final two
   digits before trailing zeros. The function returns the n‑th such number
   (n ≥ 1). It maintains a static cache of already found terms and
   extends it on demand. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Helper: fast modular exponentiation with modulus 100. */
static seq_t modpow2(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 2;
    while (exp) {
        if (exp & 1)
            result = (seq_t)((uint64_t)result * base % 100);
        base = (seq_t)((uint64_t)base * base % 100);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* static state that is kept between calls */
    static idx_t  i        = 1;   /* last processed integer (n!) */
    static idx_t  c2       = 0;   /* total exponent of 2 counted so far */
    static idx_t  c5       = 0;   /* total exponent of 5 counted so far */
    static seq_t  prod     = 1;   /* product of numbers after removing 2s and 5s,
                                   modulo 100 */
    static seq_t *cache    = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* Grow the cache as needed */
    while (cache_len < n) {
        /* Advance to the next integer */
        i++;

        /* Count 2s and 5s in i, leaving the rest in x */
        idx_t x = i;
        while ((x & 1) == 0) { c2++; x >>= 1; }
        while (x % 5 == 0)   { c5++; x /= 5; }

        /* Update the running product modulo 100 */
        prod = (seq_t)((uint64_t)prod * (x % 100) % 100);

        /* Compute current value of n! / 10^z modulo 100 */
        seq_t p_val = (seq_t)((uint64_t)prod * modpow2(c2 - c5) % 100);

        /* Store the number if the two final digits before trailing zeros are 36 */
        if (p_val == 36) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;               /* memory failure – give up */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = i;
        }
    }

    return cache[n - 1];
}
