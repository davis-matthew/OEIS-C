/*
 * A033237  Primes of form x^2+58*y^2.
 * https://oeis.org/A033237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-th prime p such that p ≡
 *            1,9,25,33,35,49,51,57,59,65,67,81,83,91,107,115,121,123,129,139,161,169,179,187,209,219,225,227
 *            (mod 232)
 * Range:     u8 n <= 9, u16 n <= 1207, u32 n <= 1208, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

static const unsigned char residues[28] = {
    1, 9, 25, 33, 35, 49, 51, 57, 59, 65,
    67, 81, 83, 91, 107, 115, 121, 123, 129, 139,
    161, 169, 179, 187, 209, 219, 225, 227
};

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static void ensure_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap << 1 : 64;
    while (newcap < needed) newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    cache_cap = newcap;
}

static bool matches_residue(seq_t p)
{
    for (int i = 0; i < 28; ++i)
        if ((p % (seq_t)232) == residues[i])
            return true;
    return false;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for offset 0; safety */
    while (cache_size < (size_t)n) {
        /* Generate next candidate prime */
        seq_t p = (n == 1) ? 3 : cache[cache_size - 1] + 2;
        while (true) {
            if (matches_residue(p) && is_prime_big(p)) {
                ensure_cap(cache_size + 1);
                cache[cache_size++] = p;
                break;
            }
            p += 2;  /* only odd numbers */
        }
    }
    return cache[n - 1];
}
