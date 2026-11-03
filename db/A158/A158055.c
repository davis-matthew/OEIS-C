/*
 * A158055  a(1)=2, a(n+1) is the smallest prime > n*first digit of a(n).
 * https://oeis.org/A158055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(1) = 2, a(n+1) = the smallest prime strictly > n *
 *            first_digit(a(n))
 * Range:     u8 n <= 31, u16 n <= 8194, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9936 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <stdbool.h>  /* bool */
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

/* Cache for already computed terms (0‑based: a(1) at cache[0]) */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Return the first decimal digit of x (0 ≤ x). */
static seq_t first_digit(seq_t x)
{
    while (x >= (seq_t)10) {
        x = x / (seq_t)10;
    }
    return x;   /* 0‑9 */
}

/* Grow the cache capacity to at least needed elements. */
static void ensure_capacity_size(size_t needed)
{
    if (needed < cache_cap) {
        return;
    }
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= needed) {
        new_cap *= 2;
    }
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    if (!new_cache) {
        abort();  /* allocation failure */
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Computes a(n) for any n >= 1, with memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;   /* undefined for n < 1, defensive return */
    }

    size_t idx = (size_t)n - 1;   /* convert to 0‑based index */

    /* Compute any missing terms up to idx. */
    if (idx >= cache_len) {
        ensure_capacity_size(idx);
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i == 0) {           /* a(1) */
                cache[0] = (seq_t)2;
                continue;
            }

            /* Previous value a(i) (since i is 0‑based, a(i) = cache[i-1]) */
            seq_t prev = cache[i - 1];

            /* First decimal digit of a(i) */
            seq_t fd = first_digit(prev);

            /* Compute n * first_digit, with n = i+1 */
            seq_t prod = ((seq_t)i) * fd;   /* i is 0‑based, matches n-1 */

            /* Search for the smallest prime > prod */
            seq_t candidate = prod + (seq_t)1;
            while (!is_prime_big(candidate)) {
                candidate += (seq_t)1;
            }

            cache[i] = candidate;  /* store a(i+1) */
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
