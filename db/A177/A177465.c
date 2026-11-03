/*
 * A177465  Primes of the form (35*10^k-71)/9.
 * https://oeis.org/A177465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth prime of the form (35*10^k - 71)/9
 * Range:     u8 none, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Validated: partial, pass, 7 terms through n = 7 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#include <stdbool.h>
#endif

/* Static state for caching computed primes. */
static seq_t pow10_val = 1;   /* 10^current_k */
static idx_t current_k = 0;   /* current exponent of 10 */
static idx_t next_k = 1;      /* next exponent to test */

#ifdef __cplusplus
static std::vector<seq_t> primes;
#else
static seq_t *primes = NULL;
static size_t primes_cap = 0;
static size_t primes_count = 0;
#endif

/* Helper to advance pow10_val to 10^target_k. */
static void advance_pow10(idx_t target_k) {
    while (current_k < target_k) {
        pow10_val = mul_small(pow10_val, 10);
        current_k++;
    }
}

/* Store a new prime into the cache. */
static void store_prime(seq_t val) {
#ifdef __cplusplus
    primes.push_back(val);
#else
    if (primes_count >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 1;
        seq_t *tmp = realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_count++] = val;
#endif
}

/* Small helpers to query cache size/value. */
#ifdef __cplusplus
static inline size_t primes_size(void) { return primes.size(); }
static inline seq_t primes_value_at(size_t idx) { return primes[idx]; }
#else
static inline size_t primes_size(void) { return primes_count; }
static inline seq_t primes_value_at(size_t idx) { return primes[idx]; }
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* Undefined, but return 0 for safety. */

    /* Generate primes until we have at least n of them. */
    while (primes_size() < (size_t)n) {
        /* Ensure pow10_val equals 10^next_k. */
        advance_pow10(next_k);

        /* Compute candidate: (35 * 10^k - 71) / 9. */
        seq_t candidate = pow10_val * 35;
        candidate -= 71;
        candidate /= 9;

        if (is_prime_big(candidate)) {
            store_prime(candidate);
        }

        next_k++;
    }

    return primes_value_at(n - 1);
}
