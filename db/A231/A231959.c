/*
 * A231959  Numbers n dividing the Lucas sequence u(n) defined by u(i) =
 *          3*u(i-1) - u(i-2) with initial conditions u(0)=0, u(1)=1.
 * https://oeis.org/A231959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th integer m such that m | u(m), where u(k) is the Lucas
 * sequence defined by u(0)=0, u(1)=1, u(k)=3*u(k-1)-u(k-2).
 * Equivalently, m | F_{2m} with F the Fibonacci sequence.
 * The implementation searches successive m and tests
 * m | F_{2m} using fast‑doubling Fibonacci modulo m.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* multiplication modulo mod using 128‑bit intermediate
 * to avoid overflow on 64‑bit machines. */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* F(n) modulo mod using fast doubling.  n and mod are 64‑bit values. */
static inline uint64_t fib_mod_uint64(uint64_t n, uint64_t mod) {
    if (mod == 1) return 0;
    uint64_t a = 0;  /* F(0) */
    uint64_t b = 1;  /* F(1) */
    for (int i = 63; i >= 0; --i) {
        /* Compute (F(2k), F(2k+1)) from (F(k)=a, F(k+1)=b) */
        uint64_t two_b = mul_mod_uint64(b, 2, mod);
        uint64_t two_b_minus_a = (two_b + mod - a) % mod;
        uint64_t c = mul_mod_uint64(a, two_b_minus_a, mod);                  /* F(2k) */
        uint64_t d = (mul_mod_uint64(a, a, mod) + mul_mod_uint64(b, b, mod)) % mod; /* F(2k+1) */
        if ((n >> i) & 1ULL) { /* go to k = 2k+1 */
            a = d;
            b = (c + d) % mod;
        } else { /* stay at k = 2k */
            a = c;
            b = d;
        }
    }
    return a % mod;
}

/* Returns true iff m divides u(m).  Since u(m)=F_{2m}, this tests
 * whether F_{2m} ≡ 0 (mod m). */
static inline bool n_divides_u(uint64_t m) {
    if (m == 0) return false;
    return fib_mod_uint64(2ULL * m, m) == 0;
}

/* Main sequence function, with memoisation for arbitrary ordering of
 * requests. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_candidate = 2;  /* we already have 1 in cache */

    if (cache == NULL) {                /* one‑time initialisation */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 1;
        cache[0] = (seq_t)1;            /* a(1) = 1 */
        next_candidate = 2;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* search for the next m that satisfies the divisibility */
        while (true) {
            if (n_divides_u(next_candidate)) {
                if (cache_len >= cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
