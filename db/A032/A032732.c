/*
 * A032732  Numbers k such that k prefixed by '7' and followed by '9' is
 *          prime.
 * https://oeis.org/A032732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 995 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032732: a(n) is the n‑th integer k such that 7 k 9 is prime. */

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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_k = 0;          /* next k to test */
static idx_t next_threshold = 10; /* 10^digits(k)   */
static seq_t pow10 = 100;         /* 10^(digits(k)+1) */

/* Ensure the cache can store at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, 0 for safety */

    /* Generate terms until the cache holds at least n entries */
    while (cache_len < (size_t)n)
    {
        /* Adjust digit length when k reaches the next threshold 10^d */
        if (next_k == next_threshold) {
            next_threshold *= 10;      /* 10^d → 10^(d+1) */
            pow10      *= 10;          /* 10^(d+1) → 10^(d+2) */
        }

        /* Candidate number: 7 * 10^(digits+1) + k * 10 + 9 */
        seq_t candidate = 7 * pow10;
        candidate += ((seq_t)next_k) * 10;
        candidate += 9;

        if (is_prime_big(candidate)) {
            grow_cache(cache_len + 1);
            cache[cache_len] = next_k;
            ++cache_len;
        }
        ++next_k;
    }

    return cache[n - 1];
}
