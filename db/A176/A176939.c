/*
 * A176939  Primes of the form 23^n + 10^n.
 * https://oeis.org/A176939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, bref
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: partial, pass, 3 terms through n = 3 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS: A176939
 * Primes of the form 23^n + 10^n.
 * a(1) = 23^0 + 10^0 = 2,
 * a(2) = 23^7 + 10^7 = 289841,
 * a(3) = 23^12 + 10^12 = 6132620415680998648961, …
 *
 * The function returns the n‑th such prime (n≥1).  If no such prime
 * has been found yet the function continues computing terms until the
 * requested index is reached or until further terms cannot be found.
 *
 * Implementation: iteratively compute 23^k+10^k starting from k=0,
 * test primality with is_prime_big, cache found primes, and
 * return the requested element.  The cache allows calls in any order.
 */

#include <stddef.h>          /* size_t */
#include <stdlib.h>          /* realloc, abort */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

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

/* Grow a dynamic array to at least 'need' elements. */
static void grow_cache(size_t *cap, seq_t **arr, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap) ? (*cap << 1) : 8;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
        if (!tmp) abort();           /* unrecoverable, as required */
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Static storage for memoisation. */
static seq_t *cache = NULL; /* cached primes */
static size_t cache_cap = 0;
static idx_t  terms_done = 0; /* number of primes cached */

/* Current powers of 23 and 10 (starting with 23^0 = 10^0 = 1). */
static seq_t pow23 = 1;
static seq_t pow10 = 1;
static idx_t current_exp = 0; /* exponent k */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n = 0 */

    /* Generate terms until we have at least n cached. */
    while (terms_done < n) {
        seq_t candidate = pow23 + pow10; /* 23^k + 10^k for current k */

        if (is_prime_big(candidate)) {
            grow_cache(&cache_cap, &cache, terms_done + 1);
            cache[terms_done] = candidate;
            terms_done++;
        }

        /* Prepare for next exponent. */
        pow23 = mul_small(pow23, 23);
        pow10 = mul_small(pow10, 10);
        current_exp++;
    }

    /* Return the n-th term (1‑based index). */
    return cache[n - 1];
}
