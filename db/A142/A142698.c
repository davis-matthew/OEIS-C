/*
 * A142698  Primes congruent to 53 mod 57.
 * https://oeis.org/A142698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 53 modulo 57
 * Range:     u8 n <= 2, u16 n <= 180, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 977 (2026-11-03)
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

static seq_t *prime_cache = NULL;
static size_t cache_cap   = 0;
static size_t cache_len   = 0;
static uint64_t next_k    = 0;          /* index of next candidate (k in 53+57*k) */

/* returns the n-th prime p such that p % 57 == 53   (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of range for OEIS offset 1 */

    /* grow cache until at least n primes stored */
    while (cache_len < (size_t)n) {
        /* candidate = 53 + 57*next_k */
        uint64_t cand = 53ULL + 57ULL * next_k;
        ++next_k;

        /* stop if candidate exceeds the maximum representable value of seq_t
           for builtin integer types */
        static bool max_computed   = false;
        static uint64_t max_val64  = UINT64_MAX;
        if (!max_computed) {
            if (sizeof(seq_t) <= sizeof(uint64_t)) {
                seq_t max_seq = (seq_t)-1;
                max_val64 = (uint64_t)max_seq;
            }
            max_computed = true;
        }
        if (cand > max_val64) {
            /* no further primes will fit in seq_t; break to avoid infinite loop */
            break;
        }

        if (is_prime_big((seq_t)cand)) {
            /* ensure capacity (start with 16, double as needed) */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
                if (!tmp) abort();
                prime_cache = tmp;
                cache_cap   = new_cap;
            }
            prime_cache[cache_len++] = (seq_t)cand;
        }
    }

    if (cache_len >= (size_t)n) {
        return prime_cache[n - 1];
    }
    /* Fallback for requests beyond reachable range (e.g., small seq_t types) */
    return 0;
}
