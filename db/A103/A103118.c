/*
 * A103118  Numbers k such that 100*k + 57 is prime.
 * https://oeis.org/A103118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   mul_mod_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that 100*k + 57 is prime.
   a(n) = the n-th smallest k with 100*k+57 prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod_uint64.h"

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp > 0) {
        if (exp & 1)
            result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Cache of found k values ----------
 * The array is grown only as needed.  In C it uses malloc/realloc,
 * in C++ the cast is required to silence the warning.  */
static seq_t *cache    = NULL;
static idx_t  cache_sz = 0;   /* number of elements currently stored */
static uint64_t next_test = 1;  /* next candidate k to test */

seq_t a(idx_t n)
{
    /* already computed? */
    if (n > 0 && n <= cache_sz) {
        return cache[n - 1];
    }

    /* enlarge the array if necessary */
    if (n > cache_sz) {
        idx_t new_size = n;
        if (cache == NULL) {
            cache = (seq_t *)malloc((size_t)new_size * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        }
    }

    /* test k values until we have at least n entries */
    uint64_t k = next_test;
    while (cache_sz < n) {
        uint64_t val = 100ULL * k + 57ULL;
        if (is_prime_uint64(val)) {
            cache[cache_sz++] = (seq_t)k;
        }
        ++k;
    }
    next_test = k;   /* remember where we left off for future calls */

    return cache[n - 1];
}
