/*
 * A129495  Composite numbers k such that 5^k (mod k) is a power of 5 greater
 *          than 1.
 * https://oeis.org/A129495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: composite numbers k such that 5^k (mod k) is a power of 5 greater than 1 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"

/* Compute 5^exp mod mod.  Uses 128‑bit arithmetic to avoid overflow. */
static inline seq_t modpow5(idx_t exp, idx_t mod)
{
    unsigned __int128 result = 1;
    unsigned __int128 base   = 5 % mod;
    while (exp)
    {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

/* Cached sequence storage.  200 000 slots is far more than the 10 000 terms
   that will be requested.  The array is static, so it works for both
   integer and arbitrary‑precision seq_t. */
#define CACHE_CAPACITY 200000
static seq_t  seq_cache[CACHE_CAPACITY];
static idx_t  seq_size   = 0;   /* number of terms currently stored */
static idx_t  max_checked = 0; /* highest k examined so far */

/* Extend the cache until it contains at least 'need' terms. */
static void extend_cache(idx_t need)
{
    while (seq_size < (size_t)need)
    {
        idx_t k = ++max_checked;
        if (k < 2) continue;           /* skip 0 and 1 */
        if (is_prime(k)) continue;     /* we only want composites */

        seq_t r = modpow5(k, k);        /* 5^k mod k */
        if (r <= 1) continue;          /* 0 or 1 cannot be a power > 1 of 5 */

        seq_t t = r;                    /* test if r is a power of 5 */
        while (t % 5 == 0) t /= 5;
        if (t != 1) continue;           /* not a pure power of 5 */

        if (seq_size == CACHE_CAPACITY) abort(); /* safety check */
        seq_cache[seq_size++] = k;
    }
}

/* Public function: returns the n‑th element of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but harmless */
    if (seq_size < n) extend_cache(n);
    return seq_cache[n - 1];
}
