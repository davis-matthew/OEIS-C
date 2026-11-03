/*
 * A121402  Prime values of integers written in factorial base, interpreted as
 *          in base 10.
 * https://oeis.org/A121402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth prime that equals the decimal interpretation of
 *            the factorial–base representation of some integer m (starting
 *            from m = 1)
 * Range:     u8 n <= 3, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return the n-th term of sequence A121402. */
seq_t a(idx_t n)
{
    /* Keep a static cache of all primes found so far. */
    static seq_t *cache = NULL;     /* pointer to cached primes       */
    static size_t cache_len = 0;    /* number of primes currently stored */
    static size_t cache_cap = 0;    /* allocated capacity of cache      */
    static idx_t last_m = 1;        /* last integer that was processed  */

    if (n == 0)
        return 0;   /* undefined for 0, but avoid UB */

    /* Grow the cache until it contains at least n primes. */
    while (cache_len < (size_t)n)
    {
        /* Next integer to examine. */
        ++last_m;
        idx_t t = last_m;
        int digits[64];        /* factorial–base digits (least significant first) */
        size_t len = 0;
        int i = 1;             /* position index (factorial base = i+1) */

        /* Compute the factorial base representation of last_m. */
        while (t > 0 && i < 64)
        {
            digits[len] = (int)(t % (idx_t)(i + 1));
            t /= (idx_t)(i + 1);
            ++len;
            ++i;
        }

        /* Interpret those digits as a base‑10 integer. */
        seq_t val = 0;
        for (int j = (int)len - 1; j >= 0; --j)
            val = val * 10 + (seq_t)digits[j];

        /* If the value is prime, add it to the cache. */
        if (is_prime_big(val))
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)
                {
                    /* Allocation failure – abort the program (no clean‑up). */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    return cache[(size_t)n - 1];
}
