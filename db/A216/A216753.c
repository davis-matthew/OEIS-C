/*
 * A216753  Primes p such that x^44 = -2 has no solution mod p.
 * https://oeis.org/A216753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 71 terms through n = 949 (2026-11-03)
 * Helpers:   is_prime_u64, mem, powmod_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216753 – primes p for which the congruence x^44 = -2 (mod p) has no
 * solution.  Computed by testing primes in order and storing those that violate
 * the criterion (-2)^((p-1)/gcd(44,p-1)) ≡ 1 (mod p).  Results are memoised,
 * so the order of calls is irrelevant. */
#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc */
#include <stdbool.h>  /* bool */

#include "../utils/powmod_u64.h"
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

static seq_t *prime_cache = NULL;    /* 0‑based array holding qualifying primes */
static size_t cache_size = 0;        /* number of stored elements */
static size_t cache_cap  = 0;        /* allocated capacity */
static uint64_t next_prime_to_test = 3; /* odd integer being tested */

/* Euclidean algorithm for uint64_t */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Grow the cache to accommodate at least `need` elements. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    prime_cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used but safety */
    /* Expand cache until we have at least n elements. */
    while (cache_size < (size_t)n) {
        /* Test current candidate for primality. */
        if (is_prime_big((seq_t)next_prime_to_test)) {
            uint64_t p = next_prime_to_test;
            /* Compute g = gcd(44, p-1) and exponent e = (p-1)/g. */
            uint64_t g = gcd64(44ULL, p - 1ULL);
            uint64_t e = (p - 1ULL) / g;
            /* Test whether (-2)^e ≡ 1 (mod p). */
            uint64_t test = powmod_u64((p - 2ULL), e, p);
            if (test != 1ULL) {
                /* No solution exists; store this prime. */
                ensure_cache_cap(cache_size + 1);
                prime_cache[cache_size++] = (seq_t)p;
            }
        }
        /* Advance to next odd candidate. */
        next_prime_to_test += 2ULL;
        /* Handle wrap‑around for 64‑bit overflow in the unlikely event it
         * occurs during the targeted n range. */
        if (next_prime_to_test < 3ULL) next_prime_to_test = 3;
    }
    return prime_cache[(size_t)n - 1];
}
