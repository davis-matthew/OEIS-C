/*
 * A090933  Primes such that most significant digit swapped with all other
 *          digits yields primes.
 * https://oeis.org/A090933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: primes p such that swapping the most significant digit
   with any other digit yields a prime (for every other digit). */

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

/* Helper: extract decimal digits of n into buf[0..len-1] (least significant first).
   len is set to the number of digits. */
static void digits_to_array(seq_t n, uint8_t *buf, size_t *len)
{
    size_t i = 0;
    seq_t x = n;
    while (x > (seq_t)0) {
        buf[i++] = (uint8_t)(x % (seq_t)10);
        x /= (seq_t)10;
    }
    *len = i;
}

/* Cache of already found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Test whether a prime n satisfies the digit‑swap property. */
static bool is_good(seq_t n)
{
    /* 0‑ or 1‑digit primes trivially satisfy the property. */
    if (n < (seq_t)10) return true;

    uint8_t buf[32];          /* enough for 256‑digit numbers */
    size_t len, i;
    digits_to_array(n, buf, &len);

    uint8_t msd_index = len - 1;          /* index of most significant digit */
    uint8_t msd = buf[msd_index];

    /* For each other digit, swap with the msd and test primality. */
    for (uint8_t pos = 0; pos < msd_index; ++pos) {
        /* swap digits */
        uint8_t tmp = buf[pos];
        buf[pos]     = msd;
        buf[msd_index] = tmp;

        /* rebuild the swapped number */
        seq_t val = (seq_t)0;
        for (size_t k = msd_index; k < len; --k) {
            val = val * (seq_t)10 + (seq_t)buf[k];
        }

        if (!is_prime_big(val)) {
            /* restore original digits before returning */
            buf[msd_index] = msd;
            buf[pos]       = tmp;
            return false;
        }

        /* restore original digits */
        buf[msd_index] = msd;
        buf[pos]       = tmp;
    }
    return true;
}

/* Extend the cache until it contains at least 'need' terms. */
static void ensure_terms(idx_t need)
{
    while (cache_len < need) {
        seq_t cand = (cache_len == 0) ? (seq_t)2 : cache[cache_len - 1] + (seq_t)1;

        /* Skip even numbers beyond 2. */
        if (cand != (seq_t)2) {
            while ((cand % (seq_t)2) == (seq_t)0)
                ++cand;
        }

        /* Search for the next qualifying prime. */
        while (true) {
            if (is_prime_big(cand) && is_good(cand)) {
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 4;
                    seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                    cache = tmp;
                }
                cache[cache_len++] = cand;
                break;
            }
            cand += (seq_t)2;   /* skip to the next odd candidate */
        }
    }
}

/* External function required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1 – behaviour below offset is irrelevant */
    ensure_terms(n);
    return cache[n - 1];
}
