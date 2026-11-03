/*
 * A142936  Primes congruent to 25 mod 64.
 * https://oeis.org/A142936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 191, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 60 terms through n = 991 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes congruent to 25 modulo 64.
   The function returns the n-th such prime (a(1)=89, a(2)=281, ...). */
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

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* 1‑based offset – caller must use n >= 1 */

    /* Static cache for already found primes. */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* current allocation */
    static size_t size = 0;  /* number of stored primes */

    /* Convert n to a size_t for array indexing; the value is small in practice. */
    size_t idx = (size_t)n;

    /* Start searching from the last cached prime or from the first term 89. */
    seq_t candidate;
    seq_t step = (seq_t)64;
    if (size == 0) {
        candidate = (seq_t)89;   /* first prime ≡25 (mod 64) */
    } else {
        candidate = cache[size - 1] + step;
    }

    /* Grow the cache when needed. */
    if (size == 0) {
        cap = 16;          /* initial guess */
        cache = mem_new(seq_t, cap);
    } else if (size == cap) {
        cap <<= 1;        /* double capacity */
        cache = mem_resize(seq_t, cache, cap);
    }

    /* Search until we have at least idx primes in the cache. */
    while (size < idx) {
        if (is_prime_big(candidate)) {
            if (size == cap) {                 /* resize if needed */
                cap <<= 1;
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[size++] = candidate;
        }
        candidate += step;
    }

    return cache[idx - 1];
}
