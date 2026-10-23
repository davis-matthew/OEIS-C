/*
 * A105139  Numbers k such that 512*k+257 is prime.
 * https://oeis.org/A105139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating primes of the form 512*k + 257. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Deterministic Miller‑Rabin for 64‑bit integers. */
static bool is_prime64(uint64_t n) {
    if (n < 2) return false;

    /* Test divisibility by small primes. */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Write n-1 as d * 2^s with d odd. */
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers. */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;

        /* Compute a^d mod n. */
        uint64_t x = 1;
        uint64_t base = a;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL)
                x = (uint64_t)((__uint128_t)x * base % n);
            base = (uint64_t)((__uint128_t)base * base % n);
            exp >>= 1;
        }

        if (x == 1 || x == n - 1)
            continue;

        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = (uint64_t)((__uint128_t)x * x % n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }

    return true;
}

/* Sequence A105139: k such that 512*k + 257 is prime. */
seq_t a(idx_t n) {
    static seq_t *data = NULL;       /* array of found k's   */
    static idx_t data_size = 0;      /* number of k's stored */
    static idx_t data_cap = 0;       /* capacity of 'data'   */
    static uint64_t next_k = 0;      /* next candidate k to test */

    /* Return cached value if already computed. */
    if (n <= data_size)
        return data[n - 1];

    /* Ensure enough terms are generated. */
    while (data_size < n) {
        /* Check if 512*k + 257 is prime. */
        uint64_t candidate = (next_k << 9) + 257ULL;  /* 512*next_k + 257 */
        if (is_prime64(candidate)) {
            /* Expand array if necessary. */
            if (data_cap == 0) {
                data_cap = 8;
                data = (seq_t *)malloc(sizeof(seq_t) * data_cap);
            } else if (data_size >= data_cap) {
                data_cap <<= 1;
                data = (seq_t *)realloc(data, sizeof(seq_t) * data_cap);
            }
            data[data_size++] = (seq_t)next_k;
        }
        ++next_k;
    }

    return data[n - 1];
}
