/*
 * A107157  Primes of the form x^2 + 50y^2.
 * https://oeis.org/A107157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the n-th prime that can be written as x^2 + 50 y^2. */
/* The function generates primes incrementally and stores all such primes
   in a dynamic array. It works for any integer type seq_t (built‑in or
   arbitrary‑precision) and for any index type idx_t (uint64_t). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Integer square root (floor) for 64‑bit values. */
static uint64_t isqrt64(uint64_t v)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32); /* 2^32 > sqrt(2^64-1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        if (mid > 0 && mid > v / mid) { /* mid*mid > v */
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo - 1;
}

/* Test whether a prime p can be expressed as x^2 + 50*y^2 with y ≥ 1. */
static bool representable_prime(uint64_t p)
{
    uint64_t y_max = isqrt64(p / 50ULL);
    for (uint64_t y = 1; y <= y_max; ++y) {
        uint64_t ysq = 50ULL * y * y;
        uint64_t t = p - ysq;
        uint64_t x = isqrt64(t);
        if (x * x == t)
            return true;
    }
    return false;
}

/* Dynamic array of primes (uint64_t). */
struct prime_buffer {
    uint64_t *data;
    size_t len;
    size_t cap;
};
static struct prime_buffer primes = {NULL, 0, 0};

static void ensure_prime_cap(size_t needed)
{
    if (primes.cap < needed) {
        size_t newcap = primes.cap ? primes.cap * 2 : 64;
        if (newcap < needed)
            newcap = needed * 2;
        uint64_t *tmp = (uint64_t*)realloc(primes.data, newcap * sizeof(uint64_t));
        if (!tmp) exit(1);
        primes.data = tmp;
        primes.cap = newcap;
    }
}

/* Dynamic array of terms (seq_t). */
struct term_buffer {
    seq_t *data;
    size_t len;
    size_t cap;
};
static struct term_buffer terms = {NULL, 0, 0};

static void ensure_term_cap(size_t needed)
{
    if (terms.cap < needed) {
        size_t newcap = terms.cap ? terms.cap * 2 : 64;
        if (newcap < needed)
            newcap = needed * 2;
        seq_t *tmp = (seq_t*)realloc(terms.data, newcap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms.data = tmp;
        terms.cap = newcap;
    }
}

/* Next candidate integer to test for primality. */
static uint64_t next_candidate = 2;

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; callers should use n≥1 */

    /* Generate primes until we have at least n desired terms. */
    while (terms.len < (size_t)n) {
        uint64_t p = next_candidate;
        bool is_prime = true;

        /* Skip even numbers >2. */
        if (p > 2 && (p & 1ULL) == 0ULL) {
            ++next_candidate;
            continue;
        }

        uint64_t limit = isqrt64(p);
        for (size_t i = 0; i < primes.len; ++i) {
            uint64_t q = primes.data[i];
            if (q > limit)
                break;
            if (p % q == 0) {
                is_prime = false;
                break;
            }
        }

        if (is_prime) {
            /* Store the new prime. */
            ensure_prime_cap(primes.len + 1);
            primes.data[primes.len++] = p;

            /* Check the representation condition. */
            if (representable_prime(p)) {
                ensure_term_cap(terms.len + 1);
                terms.data[terms.len++] = (seq_t)p;
            }
        }

        ++next_candidate;
    }

    return terms.data[(size_t)n - 1];
}
