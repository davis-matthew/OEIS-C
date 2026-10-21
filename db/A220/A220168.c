/*
 * A220168  Numbers k that divide Fibonacci(k+2).
 * https://oeis.org/A220168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer k such that k | F_{k+2}
 * Range:     u8 n <= 10, u16 n <= 918, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation notes:
 * The function lazily computes the sequence up to the required index.
 * For each candidate k it uses fast doubling modulo k to test whether
 * F_{k+2} ≡ 0 (mod k).  Results are cached in a static array so that
 * subsequent calls with larger or smaller indices are served efficiently.
 */

#include <stdint.h>
#include <stdlib.h>

/* Forward declaration of helper: fib_pair for modulo */
static void fib_pair(uint64_t n, uint64_t mod, uint64_t *F_n, uint64_t *F_n1);

/* Sequence function */
seq_t a(idx_t n)
{
    /* Validate input */
    if (n < 1) return 0;

    /* Static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;
    /* Track the largest k already examined */
    static uint64_t last_k = 0;

    /* Grow cache if needed */
    if (cache_capacity == 0 || cache_capacity < cache_size + 64) {
        size_t new_cap = (cache_capacity == 0) ? 64 : cache_capacity * 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure -> return 0 */
        cache = new_cache;
        cache_capacity = new_cap;
    }

    /* Compute more terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        /* Next candidate k */
        uint64_t k = ++last_k;

        /* Compute F_{k+2} mod k using fast doubling */
        uint64_t Fk2, Fk3; /* F_{k+2} and F_{k+3} */
        fib_pair((uint64_t)k + 2, k, &Fk2, &Fk3);

        /* Check divisibility */
        if (Fk2 % k == 0) {
            /* Store in cache */
            if (cache_size == cache_capacity) {
                size_t new_cap = cache_capacity * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;
                cache = tmp;
                cache_capacity = new_cap;
            }
            cache[cache_size++] = (seq_t)k;
        }
    }

    /* Return the n-th term (1-based indexing) */
    return cache[n - 1];
}

/* Fast doubling algorithm for Fibonacci numbers modulo mod.
 * Computes F_n and F_{n+1} modulo 'mod' and stores them in *F_n
 * and *F_n1 respectively.  Recursion depth is O(log n).
 */
static void fib_pair(uint64_t n, uint64_t mod, uint64_t *F_n, uint64_t *F_n1)
{
    if (n == 0) {
        *F_n = 0;
        *F_n1 = 1 % mod;
        return;
    }

    uint64_t a, b; /* a = F_k, b = F_{k+1} for k = floor(n/2) */
    fib_pair(n >> 1, mod, &a, &b);

    /* Compute F_{2k} = a * (2*b - a) */
    uint64_t t = (2 * b) % mod;
    if (t < a) t += mod; /* ensure non-negative before subtraction */
    uint64_t two_b_minus_a = (t + mod - a) % mod;
    unsigned __int128 prod = (unsigned __int128)a * two_b_minus_a;
    uint64_t f2k = (uint64_t)(prod % mod);

    /* Compute F_{2k+1} = a^2 + b^2 */
    unsigned __int128 aa = (unsigned __int128)a * a;
    unsigned __int128 bb = (unsigned __int128)b * b;
    uint64_t f2k1 = (uint64_t)((aa + bb) % mod);

    if (n & 1) {
        /* n is odd: F_n = F_{2k+1}, F_{n+1} = F_{2k} + F_{2k+1} */
        *F_n = f2k1;
        uint64_t tmp = f2k + f2k1;
        if (tmp >= mod) tmp -= mod;
        *F_n1 = tmp;
    } else {
        /* n is even: F_n = F_{2k}, F_{n+1} = F_{2k+1} */
        *F_n = f2k;
        *F_n1 = f2k1;
    }
}
