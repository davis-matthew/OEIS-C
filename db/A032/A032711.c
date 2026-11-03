/*
 * A032711  Numbers k such that k prefixed by '2' and followed by '3' is
 *          prime.
 * https://oeis.org/A032711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 302, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 826 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = k where k ≥ 1 and the decimal number 2‖k‖3 is prime,
 * with ‖ reading meaning concatenation.  For example, k = 8 gives 283.
 */

/* include required helpers */
#include <stddef.h>        /* size_t */
#include <stdlib.h>       /* abort */
#include <stdbool.h>      /* bool */
#include <stdint.h>
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

/* static state for memoised generation */
static seq_t *cache = NULL;      /* cached k values */
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* allocated capacity */
static uint64_t next_k = 1;      /* next integer to test */

/* ensure the cache can accommodate at least len entries */
static void grow_cache(size_t len)
{
    if (len > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < len) new_cap = len;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }
}

/* Return the n-th term (n ≥ 1).  Undefined behavior for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range, return 0 */

    /* Expand cache until we have enough terms. */
    while (cache_len < n) {
        uint64_t k = next_k++;                     /* candidate k */
        int d = num_digits((idx_t)k);               /* number of decimal digits of k */

        /* Construct the number 2‖k‖3 = 2 * 10^(d+1) + k * 10 + 3. */
        seq_t p = 2 * pow10((idx_t)(d + 1))          /* 2 * 10^(d+1) */
                  + (seq_t)k * 10                    /* + k * 10 */
                  + 3;                               /* + 3 */

        /* Test primality. */
        if (is_prime_big(p)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
