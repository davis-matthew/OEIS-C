/*
 * A142489  Primes congruent to 22 mod 51.
 * https://oeis.org/A142489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime congruent to 22 (mod 51)
 * Range:     u8 n <= 1, u16 n <= 205, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 961 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
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

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

/* Static cache of computed primes. */
static seq_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_len = 0;
static idx_t next_k = 1;               /* next index k for candidate 51*k+22 */

/* Append a prime to the cache, growing the array if necessary. */
static void add_prime(seq_t p) {
    if (primes_len == primes_cap) {
        size_t new_cap = primes_cap == 0 ? 8 : primes_cap * 2;
        seq_t *tmp = seq_realloc(primes, primes_cap, new_cap);
        if (!tmp) abort();             /* allocation failure */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/* Return the nth prime that is congruent to 22 modulo 51. */
seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1, but guard anyway */

    /* Return from cache if already computed. */
    if ((size_t)n <= primes_len)
        return primes[n - 1];

    /* Generate primes until we have at least n of them. */
    while (primes_len < (size_t)n) {
        uint64_t candidate = 51ull * next_k + 22ull; /* 51*k + 22 */
        if (is_prime_big((seq_t)candidate))
            add_prime((seq_t)candidate);
        ++next_k;
    }

    return primes[n - 1];
}
