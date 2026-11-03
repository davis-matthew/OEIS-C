/*
 * A033220  Primes of form x^2+30*y^2.
 * https://oeis.org/A033220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime that can be written as x^2 + 30*y^2
 *            (equivalently primes ≡ 1, 31, 49, or 79 (mod 120))
 * Range:     u8 n <= 5, u16 n <= 791, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 52 terms through n = 9691 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
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
static size_t cache_cnt   = 0;
static uint64_t next_candidate = 2;   /* next integer to test */

/* Ensure the cache can hold at least `min_cap` entries. */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap)
        return;
    size_t new_cap = cache_cap ? (cache_cap << 1) : 4;
    if (new_cap < min_cap)
        new_cap = min_cap;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    prime_cache = tmp;
    cache_cap   = new_cap;
}

/* Return the n-th prime of the form x^2 + 30*y^2 (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    /* Return cached value if already computed. */
    if ((size_t)n <= cache_cnt)
        return prime_cache[(size_t)(n - 1)];

    /* Residues modulo 120 that satisfy the form. */
    const uint8_t residues[4] = { 1, 31, 49, 79 };

    /* Find additional primes until we have at least n of them. */
    while (cache_cnt < (size_t)n) {
        uint64_t cand   = next_candidate;
        uint8_t rem     = cand % 120;

        if (rem == residues[0] || rem == residues[1] ||
            rem == residues[2] || rem == residues[3]) {

            /* Only odd numbers need testing (all residues above are odd). */
            if (is_prime_big((seq_t)cand)) {
                grow_cache(cache_cnt + 1);
                prime_cache[cache_cnt++] = (seq_t)cand;
                if (cache_cnt >= (size_t)n) {
                    /* Advance the candidate to avoid re‑testing the same number. */
                    next_candidate = cand + 1;
                    break;
                }
            }
        }
        next_candidate = cand + 1;
    }

    return prime_cache[(size_t)(n - 1)];
}
