/*
 * A236395  a(n) = Fibonacci(p) mod p^2, where p = prime(n).
 * https://oeis.org/A236395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(prime(n)) mod prime(n)^2
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6152, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 75 terms through n = 9883 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

/* Multiply two seq_t values modulo mod using 128‑bit arithmetic
   to avoid overflow when seq_t == uint64_t. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)(((unsigned __int128)a * b) % mod);
}

/* Fast doubling method: compute (F(k), F(k+1)) modulo mod. */
static void fib_pair(idx_t k, seq_t mod, seq_t *f, seq_t *f_next)
{
    if (k == 0) {
        *f = 0;
        *f_next = 1;
        return;
    }
    seq_t a, b;
    fib_pair(k >> 1, mod, &a, &b);

    /* c = F(2t) = F(t) * (2*F(t+1) – F(t))  */
    seq_t two_b = mul_mod(2, b, mod);                       /* 2*b mod mod */
    seq_t two_b_minus_a = (two_b + mod - a) % mod;         /* (2*b - a) mod mod */
    seq_t c = mul_mod(a, two_b_minus_a, mod);               /* a * (2*b - a) */

    /* d = F(2t+1) = F(t)^2 + F(t+1)^2  */
    seq_t a_sq = mul_mod(a, a, mod);
    seq_t b_sq = mul_mod(b, b, mod);
    seq_t d = (a_sq + b_sq) % mod;

    if ((k & 1) == 0) {
        *f = c;
        *f_next = d;
    } else {
        *f = d;
        *f_next = (c + d) % mod;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, return 0 as a fallback */
    uint64_t p = get_prime(n);          /* prime(n), 1-indexed */
    seq_t mod = (seq_t)p * (seq_t)p;    /* p^2 */
    seq_t fn, fn_next;
    fib_pair((idx_t)p, mod, &fn, &fn_next); /* compute F(p) and F(p+1) */
    return fn % mod;                       /* fn already < mod, but kept for clarity */
}
