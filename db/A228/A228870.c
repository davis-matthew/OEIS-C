/*
 * A228870  Numbers k such that 2 * (1^k + 2^k + 3^k + ... + k^k) is not 0
 *          (mod k).
 * https://oeis.org/A228870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A228870: Numbers k such that 2 * (1^k + 2^k + … + k^k) is not 0 (mod k).
 *  Equivalent to: k has an odd prime divisor p with (p-1) | k.
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
/* NOTE:  idx_t and seq_t are defined by the build.  idx_t is unsigned 64‑bit. */

static inline bool has_property(idx_t k)
{
    /* k is zero or one → not in the sequence */
    if (k < 6) return false;
    uint64_t tmp = (uint64_t)k;

    /* strip all factors of 2 – they do not contribute to odd primes */
    while ((tmp & 1U) == 0) tmp >>= 1;

    /* test odd prime divisors up to sqrt(tmp) */
    for (uint64_t p = 3; p <= tmp / p; p += 2)
    {
        if (tmp % p == 0)
        {
            /* p is an odd prime divisor of k */
            if (k % (p - 1) == 0) return true;
            while (tmp % p == 0) tmp /= p;   /* remove p completely */
        }
    }

    /* if a prime factor > sqrt(tmp) remains it is odd */
    if (tmp > 1)
    {
        if (k % (tmp - 1) == 0) return true;
    }
    return false;
}

/* ---------- cache for the sequence ---------- */

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored values          */
static size_t cache_cap = 0;   /* current capacity of the array    */
static idx_t  next_k    = 1;   /* next integer to test             */

static void ensure_capacity(size_t needed)
{
    if (cache_len + needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (cache_len + needed > new_cap) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();            /* memory exhausted */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1 */
    while (cache_len < (size_t)n)
    {
        /* find the next qualifying integer k */
        while (true)
        {
            if (has_property(next_k))
            {
                ensure_capacity(1);
                cache[cache_len++] = (seq_t)next_k;
                ++next_k;              /* move past the one just stored */
                break;
            }
            ++next_k;
        }
    }
    return cache[n - 1];
}
