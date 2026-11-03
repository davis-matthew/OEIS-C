/*
 * A070851  Smallest prime == 1 mod (7n).
 * https://oeis.org/A070851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime p such that p ≡ 1 (mod 7n)
 * Range:     u8 n <= 10, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 67 terms through n = 494 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

seq_t a(idx_t n)
{
    /* Cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_len   = 0;

    if (n == 0) return 0;          /* undefined for n<1, just return 0 */

    /* Resize cache if necessary */
    if (n > cache_size) {
        idx_t new_size = (cache_size ? cache_size * 2 : 16);
        if (new_size < n) new_size = n;
        cache = mem_resize(seq_t, cache, (size_t)new_size);
        cache_size = new_size;
    }

    /* Compute any missing values up to n */
    for (idx_t i = cache_len; i < n; ++i) {
        idx_t idx = i + 1;                 /* 1‑based index */
        seq_t mod = (seq_t)idx * (seq_t)7;  /* 7*n */
        seq_t cand = mod + 1;              /* start with 1+7*n */

        /* Find the first prime in the arithmetic progression */
        while (!is_prime_big(cand))
            cand += mod;

        cache[i] = cand;
    }
    cache_len = n;   /* all entries up to n are known */

    return cache[n - 1];
}
