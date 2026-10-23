/*
 * A155785  Nonprimes where the first digit equals the final digit and also
 *          equals the number of digits.
 * https://oeis.org/A155785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = first n non‑prime numbers that start and end with the
 *            same digit d and have exactly d digits (sequence A155785). The
 *            sequence is finite; the last term is 999999999. The
 *            implementation generates numbers in ascending order, testing
 *            only those with first/last digit d and exactly d digits, and...
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Deterministic Miller–Rabin for all n < 2^32.
   Uses bases 2, 7, 61 which are sufficient for 32‑bit integers. */
static bool is_prime_uint32(uint32_t n)
{
    if (n < 2U) return false;
    /* small prime trial division */
    static const uint32_t small_primes[] = {
        2U, 3U, 5U, 7U, 11U, 13U, 17U, 19U, 23U,
        29U, 31U, 37U
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint32_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 = d * 2^s with d odd */
    uint32_t d = n - 1U;
    int s = 0;
    while ((d & 1U) == 0U) {
        d >>= 1U;
        ++s;
    }

    static const uint32_t bases[] = { 2U, 7U, 61U };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint32_t a = bases[i];
        if (a % n == 0U) return true;   /* n divides base → prime */

        uint64_t x = 1U;
        uint64_t base = a % n;
        uint32_t e = d;
        while (e) {
            if (e & 1U) x = (x * base) % n;
            base = (base * base) % n;
            e >>= 1U;
        }

        if (x == 1U || x == n - 1U) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1U) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    /* static cache of terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    /* generation state: current digit d and middle index */
    static idx_t gen_d = 1;
    static idx_t gen_mid = 0;
    static bool finished = false;

    /* pre‑compute powers of 10 up to 10^9 */
    static seq_t pow10[10];
    static bool pow10_init = false;
    if (!pow10_init) {
        pow10[0] = (seq_t)1;
        for (idx_t i = 1; i < 10; ++i) pow10[i] = pow10[i-1] * (seq_t)10;
        pow10_init = true;
    }

    if (n == 0) return 0;                 /* unused index */

    /* extend cache until it contains at least n terms */
    while (cache_len < n && !finished) {
        /* ensure enough space */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        seq_t candidate = 0;
        if (gen_d == 1) {                 /* 1‑digit case */
            candidate = (seq_t)1;
            cache[cache_len++] = candidate;
            gen_d = 2;
        } else if (gen_d == 2) {           /* 2‑digit case */
            candidate = (seq_t)22;
            cache[cache_len++] = candidate;
            gen_d = 3;
            gen_mid = 0;
        } else {                          /* d ≥ 3 */
            idx_t d = gen_d;
            idx_t mid_limit = (idx_t)pow10[d-2];  /* 10^(d-2) */
            if (gen_mid < mid_limit) {
                candidate = (seq_t)d * pow10[d-1] + (seq_t)gen_mid * (seq_t)10 + (seq_t)d;
                if ((d & 1U) == 0U || d == 5U) {
                    /* even d or d==5 ⇒ composite */
                    cache[cache_len++] = candidate;
                } else {
                    /* need primality test, but candidate < 1e9 */
                    uint32_t num = (uint32_t)(unsigned long long)candidate;
                    if (!is_prime_uint32(num))
                        cache[cache_len++] = candidate;
                }
                ++gen_mid;
            } else {
                ++gen_d;
                gen_mid = 0;
            }
        }

        /* stop after d>9 (max digit length 9) */
        if (gen_d > 9) {
            finished = true;
            break;
        }
    }

    if (n - 1 < cache_len) return cache[n-1];
    return 0;  /* out of bounds – sequence is finite */
}
