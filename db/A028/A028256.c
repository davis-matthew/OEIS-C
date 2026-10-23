/*
 * A028256  Fibonacci(n+2) mod n-th prime.
 * https://oeis.org/A028256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n+2) mod nth prime
 * Range:     u8 n <= 41, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes primes up to the required index
   using a simple sieve and then evaluates Fibonacci(n+2) modulo that prime
   with fast doubling modulo a small modulus.  */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static uint32_t *primes = NULL;   /* array of primes */
static size_t   primes_cnt = 0;   /* number of primes stored */

/* Allocate or extend primes so that at least `n` primes are available.
   The routine recomputes the sieve from scratch for simplicity. */
static void ensure_primes(idx_t n) {
    if (primes_cnt >= (size_t)n) return;  /* already enough */

    size_t limit = 200000;                /* initial limit, covers n <= 10000 */
    while (1) {
        /* Sieve of Eratosthenes up to `limit` */
        uint8_t *sieve = (uint8_t *)malloc(limit + 1);
        if (!sieve) return;                   /* allocation failed, give up */
        memset(sieve, 1, limit + 1);
        sieve[0] = sieve[1] = 0;

        size_t sq = (size_t)sqrt((double)limit);
        for (size_t i = 2; i <= sq; ++i) {
            if (sieve[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    sieve[j] = 0;
            }
        }

        /* Count primes to allocate array */
        size_t cnt = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (sieve[i]) ++cnt;

        /* Reallocate primes array */
        uint32_t *new_primes = (uint32_t *)realloc(primes, cnt * sizeof(uint32_t));
        if (!new_primes) { free(sieve); return; }   /* allocation failed */
        primes = new_primes;

        /* Store primes */
        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (sieve[i]) primes[idx++] = (uint32_t)i;

        free(sieve);
        primes_cnt = cnt;

        if (primes_cnt >= (size_t)n) break;  /* enough primes */
        limit *= 2;                          /* not enough, double limit */
    }
}

/* Multiplication modulo `mod` using a 64‑bit intermediate to avoid overflow. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod) {
    return (seq_t)(((unsigned long long)a * (unsigned long long)b) % (unsigned long long)mod);
}

/* Fast doubling Fibonacci modulo `mod`.  n is non‑negative. */
static seq_t fib_mod(idx_t n, seq_t mod) {
    if (n == 0) return 0;
    seq_t a = 0, b = 1;          /* F(0), F(1) */
    /* Find highest set bit */
    idx_t shift = 0;
    for (idx_t t = n; t > 1; t >>= 1) ++shift;

    for (idx_t i = shift; ; --i) {
        /* d = a * (2*b - a) mod mod   (F(2k)) */
        seq_t two_b = b + b;
        if (two_b >= mod) two_b -= mod;
        seq_t two_b_minus_a = (two_b >= a) ? two_b - a : two_b + mod - a;
        seq_t d = mul_mod(a, two_b_minus_a, mod);

        /* e = a^2 + b^2 mod mod   (F(2k+1)) */
        seq_t e = mul_mod(a, a, mod) + mul_mod(b, b, mod);
        if (e >= mod) e -= mod;

        if ((n >> i) & 1) {
            a = e;
            seq_t sum = d + e;
            if (sum >= mod) sum -= mod;
            b = sum;
        } else {
            a = d;
            b = e;
        }

        if (i == 0) break;  /* last iteration */
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* defined for n>=1 only */
    ensure_primes(n);             /* make sure nth prime is available */
    seq_t mod = primes[n - 1];     /* nth prime (0‑based) */
    return fib_mod(n + 2, mod);   /* Fibonacci(n+2) mod nth prime */
}
