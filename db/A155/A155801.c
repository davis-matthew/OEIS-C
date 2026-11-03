/*
 * A155801  Nontrivial "Strobogrammatic" primes, the same "upside-down" in at
 *          least one base b with 2 <= b <= 10.
 * https://oeis.org/A155801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = nth prime that is strobogrammatic in at least one base 2
 *            ≤ b ≤ 10
 * Range:     u8 n <= 14, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Mapping of decimal digits to their 180° rotation image.
   -1 denotes an invalid digit. */
static const int ROTATE_MAP[10] = { 0, 1, -1, -1, -1, -1, 9, -1, 8, 6 };

/* Test whether n is strobogrammatic in a single base. */
static bool is_strobogrammatic_in_base(seq_t n, int base)
{
    int digits[256];
    int len = 0;

    if (n == 0) return false;

    seq_t tmp = n;
    const seq_t sb = (seq_t)base;
    while (tmp > 0) {
        int d = (int)(tmp % sb);
        digits[len++] = d;
        tmp /= sb;
    }

    for (int i = 0; i < len; ++i) {
        int j   = len - 1 - i;
        int d1  = digits[i];
        int d2  = digits[j];
        int rot = ROTATE_MAP[d2];
        if (rot == -1 || d1 != rot) return false;
    }
    return true;
}

/* Test whether n is strobogrammatic in any base 2…10. */
static bool is_strobogrammatic_any_base(seq_t n)
{
    for (int base = 2; base <= 10; ++base) {
        if (is_strobogrammatic_in_base(n, base)) return true;
    }
    return false;
}

/* Return the nth strobogrammatic prime (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t cache_cnt   = 0;
    static seq_t cur         = 2;        /* next prime candidate */

    /* Ensure sufficient storage. */
    ensure_cache(n, &cache, &cache_cap);

    /* Generate primes until we have n terms. */
    while (cache_cnt < n) {
        if (is_prime_big(cur) && is_strobogrammatic_any_base(cur)) {
            cache[cache_cnt++] = cur;
        }

        seq_t prev = cur;
        if (cur == 2) cur = 3;
        else          cur += 2;            /* skip even numbers            */

        /* Detect wrap‑around on unsigned seq_t. */
        if (cur <= prev) break;
    }

    if (cache_cnt >= n) return cache[(size_t)(n - 1)];
    /* If the sequence cannot be continued within the capacity of seq_t,
       return 0 as a safe failure value. */
    return (seq_t)0;
}
