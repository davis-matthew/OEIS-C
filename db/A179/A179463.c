/*
 * A179463  Semiprimes A001358 containing at least one semiprime digit in base
 *          10.
 * https://oeis.org/A179463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A179463: Semiprimes containing at least one 4, 6 or 9 digit in base 10
   The function computes the n-th such semiprime by testing integers sequentially
   and caching the found values. It uses trial division to test for semiprimality
   and a simple digit scan to check for the required digits. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* true if a decimal representation of x contains 4, 6 or 9 */
static bool has_semiprime_digit(uint64_t x)
{
    while (x > 0) {
        switch (x % 10) {
        case 4: case 6: case 9:
            return true;
        }
        x /= 10;
    }
    return false;
}

/* true if n is a semiprime (product of exactly two primes, possibly equal) */
static bool is_semiprime(uint64_t n)
{
    if (n < 4)          /* smallest semiprime is 4 = 2*2            */
        return false;

    /* Trial division for the smallest prime factor */
    uint64_t p = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            p = d;
            break;
        }
    }
    if (!p)             /* n is prime */
        return false;

    uint64_t m = n / p; /* other factor */
    /* m must be prime for n to be a semiprime */
    return is_prime_big((seq_t)m);
}

seq_t a(idx_t n)
{
    static uint64_t *cache = NULL;   /* dynamic array of found terms        */
    static size_t cache_cap = 0;     /* allocated capacity                  */
    static size_t cache_size = 0;    /* number of terms stored              */
    static uint64_t last_checked = 3;/* last integer examined (n < 4 are skipped) */

    if (n == 0)  /* the OEIS offset starts at 1; the behaviour for n==0 is irrelevant */
        return (seq_t)0;

    /* Grow the cache until it contains at least n entries */
    while (cache_size < (size_t)n) {
        ++last_checked;                 /* examine the next integer */

        if (is_semiprime(last_checked) && has_semiprime_digit(last_checked)) {
            /* Allocate more space if necessary */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
                uint64_t *tmp = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
                if (!tmp) abort();                    /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
    }

    /* Return the (n‑1)-th element (array is 0‑based) */
    return (seq_t)cache[n - 1];
}
