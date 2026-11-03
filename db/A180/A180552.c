/*
 * A180552  Primes that become a different prime under the mapping 5 <=> 7.
 * https://oeis.org/A180552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 411, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 978 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A180552
 *  Primes p such that the digit mapping 5 ↔ 7 applied to the decimal
 *  expansion of p yields a different prime.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

/* Map digits 5 and 7 in a decimal representation of a seq_t. */
static seq_t map_5_7(seq_t n)
{
    seq_t result = 0;
    seq_t pow10 = 1;
    seq_t tmp = n;
    while (tmp > 0) {
        uint8_t digit = (uint8_t)(tmp % 10);
        uint8_t mapped;
        if (digit == 5)
            mapped = 7;
        else if (digit == 7)
            mapped = 5;
        else
            mapped = digit;
        result += (seq_t)mapped * pow10;
        pow10 *= 10;
        tmp /= 10;
    }
    return result;
}

/* Return the n‑th term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0, but return 0 */

    struct cache_s {
        seq_t *data;
        size_t len;
        size_t cap;
    };

    static struct cache_s cache = {0, 0, 0};
    static seq_t last_checked = 2; /* start from the first prime candidate */

    /* Allocate cache on first use. */
    if (cache.data == NULL) {
        cache.cap = 128;
        cache.data = mem_new(seq_t, cache.cap);
        if (!cache.data)
            abort();
    }

    /* Extend cache until we have at least n terms. */
    while (cache.len < n) {
        /* Test current candidate. */
        if (is_prime_big(last_checked)) {
            seq_t mapped = map_5_7(last_checked);
            if (mapped != last_checked && is_prime_big(mapped)) {
                /* Store the candidate. */
                if (cache.len == cache.cap) {
                    size_t new_cap = cache.cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache.data, new_cap);
                    if (!tmp)
                        abort();
                    cache.data = tmp;
                    cache.cap = new_cap;
                }
                cache.data[cache.len++] = last_checked;
            }
        }
        /* Move to next integer. */
        last_checked += 1;
    }

    return cache.data[n - 1];
}
