/*
 * A163627  Numbers k such that 42k + 5 is prime.
 * https://oeis.org/A163627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 677, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9146 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A163627 – numbers k such that 42*k + 5 is prime.
   This implementation searches k sequentially and caches the values.
   The function works correctly for all n ≥ 1 that fit in seq_t. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
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

/* Cache of found k values (externally represented as seq_t). */
static seq_t *k_cache      = NULL; /* array of discovered k values   */
static size_t k_cache_cap  = 0;    /* current allocation capacity   */
static size_t k_cache_cnt  = 0;    /* number of valid entries        */
static idx_t  next_k       = 0;    /* next k to test                 */

/* Ensure the cache has space for at least 'needed' elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= k_cache_cap) return;
    size_t new_cap = k_cache_cap ? k_cache_cap : 8;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, k_cache, new_cap);
    if (!tmp) abort(); /* memory allocation failure */
    k_cache = tmp;
    k_cache_cap = new_cap;
}

/* Return the n-th non‑negative integer k with 42*k + 5 prime. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* OEIS offset is 1 – undefined for n=0 */
    while (k_cache_cnt < n) {
        /* Compute 42*next_k + 5 in idx_t to avoid early truncation. */
        idx_t val_i = 42 * next_k + 5;
        seq_t val   = (seq_t)val_i;       /* cast to seq_t for primality test */
        if (is_prime_big(val)) {
            ensure_cache_cap(k_cache_cnt + 1);
            k_cache[k_cache_cnt++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return k_cache[n - 1];
}
