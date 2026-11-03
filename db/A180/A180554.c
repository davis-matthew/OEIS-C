/*
 * A180554  Primes that become a different prime under the mapping 5 <=> 9.
 * https://oeis.org/A180554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 488, u32 n <= 1595, u64 n <= 1595,
 *            u128 n <= 1595, big any n
 * Validated: standard, pass, 61 terms through n = 1487 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A180554: primes that become a different prime under the mapping 5 <=> 9 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

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

/* Map decimal digits 5 <=> 9, leaving other digits unchanged. */
static seq_t map5to9(seq_t n)
{
    if (n == 0)
        return 0;

    /* buffer large enough for any seq_t value (up to 128 bits needs < 40 decimal digits) */
    unsigned int digits[64];
    size_t len = 0;
    seq_t x = n;

    while (x != 0)
    {
        seq_t d = x % 10;
        if (d == 5)
            digits[len] = 9;
        else if (d == 9)
            digits[len] = 5;
        else
            digits[len] = (unsigned int)d;

        ++len;
        x = x / 10;
    }

    seq_t res = 0;
    for (size_t i = 0; i < len; ++i)
        res = res * 10 + digits[len - 1 - i];

    return res;
}

/* Return the n‑th term of A180554 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* safety: sequence is 1‑based */

    /* Cache of already found terms, static for successive calls. */
    static seq_t *cache = NULL;
    static size_t cached = 0;   /* how many terms have been cached */
    static size_t cap = 0;      /* current capacity of the cache array */
    static seq_t next_candidate = 2;  /* next integer to test for primality */

    /* Extend cache until it holds n terms. */
    while (cached < (size_t)n)
    {
        for (seq_t x = next_candidate; ; ++x)
        {
            if (is_prime_big(x))
            {
                seq_t y = map5to9(x);
                if (y != x && is_prime_big(y))
                {
                    /* Ensure capacity. */
                    if (cached >= cap)
                    {
                        size_t new_cap = cap ? cap << 1 : 64;
                        seq_t *tmp = mem_new(seq_t, new_cap);
                        if (!tmp)
                            abort(); /* allocation failure */
                        if (cache)
                        {
                            mem_copy(tmp, cache, cached * sizeof(seq_t));
                            mem_free(cache);
                        }
                        cache = tmp;
                        cap = new_cap;
                    }

                    cache[cached++] = x;
                    next_candidate = x + 1;

                    /* If we have reached the required position, return it. */
                    if (cached == (size_t)n)
                        return x;

                    /* Stop the inner loop and continue searching for the next term. */
                    break;
                }
            }
        }
    }

    /* Return the requested term from the cache. */
    return cache[n - 1];
}
