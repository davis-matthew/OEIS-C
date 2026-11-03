/*
 * A186102  Smallest prime p such that p == n (mod prime(n)).
 * https://oeis.org/A186102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 332, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9237 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements: a(n) = smallest prime p such that p ≡ n (mod prime(n)) */

#include <stdint.h>     /* for uint32_t, uint64_t, etc. */
#include <stdlib.h>     /* for calloc, realloc, free */
#include <math.h>       /* for log */
#include <stdbool.h>
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

/* estimate an upper bound for the n‑th prime (Rosser's theorem) */
static inline uint64_t upper_bound_for_nth_prime(idx_t n) {
    if (n < 6) return 15;
    double dn = (double)n;
    double ub = dn * (log(dn) + log(log(dn))) + 3.0;
    return (uint64_t)ub + 1;
}

/* static cache for primes */
static uint64_t *prime_cache = NULL;   /* array of primes (1‑based indexing in logic) */
static size_t prime_cache_len = 0;     /* number of primes cached */
static uint64_t sieve_limit = 0;       /* largest index sieved */

/* extend the prime cache so that it contains at least 'needed' primes */
static void ensure_prime_cache(idx_t needed) {
    if (prime_cache_len >= (size_t)needed) return;

    uint64_t new_limit = upper_bound_for_nth_prime(needed);
    if (new_limit < sieve_limit) new_limit = sieve_limit;

    /* allocate a boolean sieve (0 = prime, 1 = composite) */
    uint8_t *is_comp = (uint8_t *)calloc(new_limit + 1, 1);
    if (!is_comp) return;  /* allocation failed – leave cache unchanged */

    is_comp[0] = is_comp[1] = 1;
    uint64_t i;
    for (i = 2; i * i <= new_limit; ++i) {
        if (!is_comp[i]) {
            for (uint64_t j = i * i; j <= new_limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* count primes up to new_limit */
    size_t count = 0;
    for (i = 2; i <= new_limit; ++i)
        if (!is_comp[i]) ++count;

    /* realloc prime cache */
    uint64_t *new_cache = (uint64_t *)realloc(prime_cache, count * sizeof(uint64_t));
    if (!new_cache) {
        free(is_comp);
        return; /* allocation failed – leave cache unchanged */
    }
    prime_cache = new_cache;
    prime_cache_len = count;
    sieve_limit = new_limit;

    /* fill the cache */
    size_t idx = 0;
    for (i = 2; i <= new_limit; ++i)
        if (!is_comp[i]) prime_cache[idx++] = i;

    free(is_comp);
}

/* return the n‑th prime (1‑based), n ≥ 1 */
static inline uint64_t nth_prime(idx_t n) {
    if (n == 0) return 0;           /* defensive: offset is 1 but handle zero */
    ensure_prime_cache(n);
    return prime_cache[n - 1];
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* safety, should never be called */

    uint64_t p_n = nth_prime(n);          /* prime(n) */
    seq_t r = (seq_t)n % (seq_t)p_n;     /* remainder n mod prime(n) */

    /* the smallest prime p with p % prime(n) == r */
    seq_t candidate;
    if (r == (seq_t)0)                    /* only possible prime is prime(n) itself */
        candidate = (seq_t)p_n;
    else
        candidate = r;                    /* start with the remainder itself */

    /* search forward in steps of prime(n) until a prime is found */
    while (!is_prime_big(candidate)) {
        candidate += (seq_t)p_n;
    }
    return candidate;
}
