/*
 * A142640  Primes congruent to 54 mod 55.
 * https://oeis.org/A142640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 953 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th prime p with p ≡ 54 (mod 55). */
/* The implementation lazily generates candidates of the form 55k + 54
   and keeps those that are prime in a static array, the size of which
   is sufficient for the required range (n up to about 10000). */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
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

/* maximum number of terms we expect to need */
#define MAX_CACHE  20000

/* static cache of primes of the form 55k+54 */
static seq_t prime_cache[MAX_CACHE];
static idx_t cache_size = 0;       /* number of primes stored so far */
static idx_t last_candidate = 54;  /* last number tested */

/* public function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;          /* undefined, but harmless */
    }

    /* generate candidates until we have at least n primes cached */
    while (cache_size < n) {
        if (cache_size >= MAX_CACHE) {
            /* should not happen for the intended limits */
            return (seq_t)0;
        }

        /* next number congruent to 54 mod 55 */
        last_candidate += 55;
        if (is_prime_big((seq_t)last_candidate)) {
            prime_cache[cache_size++] = (seq_t)last_candidate;
        }
    }

    return prime_cache[n - 1];
}
