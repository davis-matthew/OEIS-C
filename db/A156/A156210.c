/*
 * A156210  Prime factors of odd Lucas numbers.
 * https://oeis.org/A156210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A156210 : prime factors of odd Lucas numbers.
    The function generates the sequence by sieving primes up to a
    fixed bound and testing each prime for the condition that it
    divides an odd Fibonacci number and an odd Lucas number.
    It stores the first 20000 such primes in a static array and
    returns the n‑th value on request.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* utilities from the test harness */
#include "../utils/jacobi_symbol.h"   /* jacobi_symbol(uint64_t a, uint64_t n) */

/* small integer square root: floor(sqrt(n)) */
static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t lo = 0, hi = n, mid, sq;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* fast doubling: returns F_n and F_{n+1} modulo mod */
static void fib_mod_pair(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1) {
    if (n == 0) { *fn  = 0; *fn1 = 1; return; }
    uint64_t a, b;
    fib_mod_pair(n >> 1, mod, &a, &b);
    uint64_t two_b = (b << 1) % mod;
    uint64_t c = (uint64_t)(((__int128)a * ((two_b + mod - a) % mod)) % mod);
    uint64_t d = (uint64_t)((( (__int128)a * a % mod) + (( (__int128)b * b % mod))) % mod);
    if (n & 1) { *fn  = d; *fn1 = (c + d) % mod; }
    else       { *fn  = c; *fn1 = d; }
}

static int cmp_uint64(const void *a, const void *b) {
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

seq_t a(idx_t n) {                     /* external linkage as required */
    /* Lazy initialisation of the sequence */
    static bool initialized = false;
    static seq_t *seq = NULL;
    static size_t seq_len = 0;

    /* Desired number of terms – more than enough for n up to 10000 */
    const size_t TARGET_TERMS = 20000;
    /* Upper bound for the sieve – should produce a sufficient list of primes */
    const size_t SIEVE_LIMIT = 25000000;   /* 25 million */

    /* Prime sieve */
    if (!initialized) {
        /* 1. Boolean array for sieve */
        uint8_t *is_prime = (uint8_t *)malloc(SIEVE_LIMIT + 1);
        memset(is_prime, 1, SIEVE_LIMIT + 1);
        is_prime[0] = is_prime[1] = 0;
        for (uint32_t i = 2; (uint64_t)i * i <= SIEVE_LIMIT; ++i) {
            if (is_prime[i]) {
                for (uint32_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_prime[j] = 0;
            }
        }
        /* 2. Count primes */
        size_t prime_cnt = 0;
        for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
            if (is_prime[i]) ++prime_cnt;
        /* 3. Store primes in an array */
        uint32_t *primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
        size_t idx = 0;
        for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i)
            if (is_prime[i]) primes[idx++] = i;
        free(is_prime);

        /* 4. Allocate storage for sequence */
        seq = (seq_t *)malloc(TARGET_TERMS * sizeof(seq_t));
        seq_len = 0;

        /* 5. Generate the sequence */
        for (size_t pi = 0; seq_len < TARGET_TERMS && pi < prime_cnt; ++pi) {
            uint32_t p = primes[pi];
            if (p == 2 || p == 5) continue;          /* special cases */

            int sigma = jacobi_symbol(5ULL, p);        /* Legendre (5|p) */
            if (sigma == 0) continue;                 /* must not happen except p==5 */

            uint64_t m = (uint64_t)p - (uint64_t)sigma;   /* p-1 or p+1 */
            uint64_t lim = isqrt_u64(m);
            /* store divisors of m in a temporary array */
            uint64_t divs[2048];
            size_t dcnt = 0;
            for (uint64_t i = 1; i <= lim; ++i) {
                if (m % i == 0) {
                    divs[dcnt++] = i;
                    uint64_t j = m / i;
                    if (j != i && dcnt < sizeof(divs)/sizeof(divs[0]))
                        divs[dcnt++] = j;
                }
            }
            qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

            uint64_t t_f = 0, t_l = 0;
            for (size_t k = 0; k < dcnt && (t_f == 0 || t_l == 0); ++k) {
                uint64_t d = divs[k];
                uint64_t f, fp1;
                fib_mod_pair(d, p, &f, &fp1);
                if (t_f == 0 && f == 0) t_f = d;
                /* L_d = 2*F_{d+1} - F_d (mod p) */
                uint64_t l = (2 * fp1 % p + p - f) % p;
                if (t_l == 0 && l == 0) t_l = d;
            }

            if (t_f && t_l && (t_f % 3) && (t_l % 3))
                seq[seq_len++] = (seq_t)p;
        }
        free(primes);
        initialized = true;
    }

    /* Return the requested term – if out of bounds, return 0 */
    if (n == 0 || (size_t)n > seq_len) return 0;
    return seq[n - 1];
}
