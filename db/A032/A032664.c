/*
 * A032664  Digit '1' concatenated with a(n) is a prime.
 * https://oeis.org/A032664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1178, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9989 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate a(n) by scanning natural numbers m and testing if
 * 1 concatenated in front of m = "1"||m is a prime.  Results are memoized
 * in a static array so that repeated and out-of-order queries are fast. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;      /* stores a(1), a(2), ... */
static size_t cache_len = 0;     /* number of cached terms      */
static size_t cache_cap = 0;     /* current capacity of cache    */
static idx_t cur_m = 1;          /* next integer to test for a(n) */

static void grow_cache_if_needed(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        if (cache == NULL)
            cache = seq_alloc(new_cap);
        else
            cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

static void ensure_n_terms(idx_t n)
{
    if (n == 0) return;
    if (cache_len >= n) return;    /* already have enough terms */

    while (cache_len < n) {
        seq_t val = (seq_t)cur_m;            /* current candidate    */
        int d   = num_digits(val);          /* number of digits in m */
        seq_t concat = pow10((idx_t)d) + val; /* 1 followed by m      */

        if (is_prime_big(concat)) {
            grow_cache_if_needed();
            cache[cache_len++] = val;     /* store a(n)            */
        }
        ++cur_m;                          /* advance to next integer  */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* not defined for n<1 */
    ensure_n_terms(n);
    return cache[n-1];
}
