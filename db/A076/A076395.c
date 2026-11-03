/*
 * A076395  Squares whose external digits form a square and whose internal
 *          digits form a prime.
 * https://oeis.org/A076395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, less, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 29 (2026-11-03)
 * Helpers:   is_prime_u64, mem, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A076395: squares whose external digits form a square and whose internal digits form a prime.
   Computed by a brute‑force search over squares. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_digits.h"
#include "../utils/mem.h"
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

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t next_m = 1;

static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    while (cache_len < (size_t)n) {
        seq_t s = (seq_t)next_m * (seq_t)next_m;
        int len = num_digits(s);
        if (len >= 3) {
            seq_t pow10len_minus1 = pow10((idx_t)(len - 1));
            seq_t first_digit = s / pow10len_minus1;
            seq_t last_digit = s % 10;
            uint64_t ext = (uint64_t)first_digit * 10u + (uint64_t)last_digit;
            if (ext == 16u || ext == 25u || ext == 36u ||
                ext == 49u || ext == 64u || ext == 81u) {
                seq_t internal = (s % pow10len_minus1) / 10;
                if (internal > 1 && is_prime_big(internal)) {
                    ensure_cache_capacity(cache_len + 1);
                    cache[cache_len++] = s;
                }
            }
        }
        ++next_m;
    }
    return cache[n - 1];
}
