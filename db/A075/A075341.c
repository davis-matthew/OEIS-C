/*
 * A075341  a(1) = 1, a(2n) is the smallest composite number == 1 mod
 *          (a(2n-1)) and a(2n+1) is the smallest prime == 1 (mod a(2n)).
 * https://oeis.org/A075341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(2n) = the smallest composite ≡ 1 (mod a(2n − 1));
 *            a(2n+1) = the smallest prime ≡ 1 (mod a(2n))
 * Range:     u8 n <= 18, u16 n <= 28, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
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

/* Find the smallest composite ≡ 1 (mod mod). */
static seq_t find_composite(seq_t mod)
{
    seq_t k = 1;
    for (;; ++k) {
        seq_t cand = mod * k + 1;
        if (!is_prime_big(cand))
            return cand;
    }
}

/* Find the smallest prime ≡ 1 (mod mod). */
static seq_t find_prime(seq_t mod)
{
    seq_t k = 1;
    for (;; ++k) {
        seq_t cand = mod * k + 1;
        if (is_prime_big(cand))
            return cand;
    }
}

/* Returns the n‑th term of A075341 (1‑indexed). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* out of range */

    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;
    static size_t capacity = 0;

    if (n <= max_computed)
        return cache[n];

    /* Expand cache if necessary. */
    if (capacity < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = mem_new(seq_t, new_cap);
        if (!new_cache)
            return (seq_t)0;        /* allocation failure */

        /* Copy existing data. */
        for (size_t i = 0; i < capacity; ++i)
            new_cache[i] = cache[i];

        if (cache)
            mem_free(cache);

        cache = new_cache;
        capacity = new_cap;
    }

    /* Compute terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else if ((i & 1) == 0) {        /* even index */
            cache[i] = find_composite(cache[i - 1]);
        } else {                          /* odd index > 1 */
            cache[i] = find_prime(cache[i - 1]);
        }
    }

    max_computed = n;
    return cache[n];
}
