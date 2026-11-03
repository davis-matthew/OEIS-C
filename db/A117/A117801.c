/*
 * A117801  Triangular numbers for which the product of the digits is a prime
 *          number.
 * https://oeis.org/A117801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  more, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-03)
 * Helpers:   digit_product, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117801: triangular numbers whose digit product is a prime.
   The function searches consecutive triangular numbers, stores the
   ones meeting the criterion in a growing cache, and returns the n‑th
   such number.  It works for any order of calls and uses only ANSI
   C/C++ standard facilities and the utility helpers listed above. */

#include <stddef.h>   /* NULL, size_t */
#include <stdlib.h>   /* malloc, realloc, free */
#include <stdbool.h>  /* bool */

#include <stdint.h>
#include "../utils/digit_product.h"
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

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* static variable to remember next triangular index to test */
    static idx_t next_k = 1;

    if (n == 0)          /* undefined for n < 1; return 0 as sentinel */
        return 0;

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n)
    {
        /* compute T_next_k = next_k*(next_k+1)/2, using seq_t arithmetic */
        seq_t t = (seq_t)next_k * ((seq_t)next_k + 1) / 2;
        next_k++;

        /* product of digits */
        seq_t prod = digit_product(t);
        if (is_prime_big(prod))
        {
            /* grow cache if needed */
            if (cache_len == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 4 : cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp)          /* out of memory – abort gracefully */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = t;
        }
    }

    return cache[n - 1];
}
