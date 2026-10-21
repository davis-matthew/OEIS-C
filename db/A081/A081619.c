/*
 * A081619  Numbers whose divisors can be arranged as equilateral triangle.
 * https://oeis.org/A081619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 5713, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A081619: numbers whose divisor number is a triangular number.
   Formula used: a(n) is the n‑th integer k>0 such that d(k) = m(m+1)/2
   for some integer m. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Integer square root for uint64_t */
static uint64_t isqrt_u64(uint64_t v)
{
    uint64_t r = (uint64_t)sqrtl((long double)v);
    while ((r + 1) * (r + 1) <= v) ++r;
    while (r * r > v) --r;
    return r;
}

/* Test if t is a triangular number: ∃m, t = m(m+1)/2 */
static bool is_triangular(uint64_t t)
{
    uint64_t d = 8ULL * t + 1;
    uint64_t s = isqrt_u64(d);
    if (s * s != d) return false;
    return ((s - 1) & 1) == 0;
}

/* Dynamic array of primes, stored as uint32_t */
static uint32_t *primes = NULL;
static size_t primes_cnt = 0, primes_cap = 0;

/* Add a prime to the primes array (it is always larger than previous ones) */
static void add_prime(uint32_t p)
{
    if (primes_cnt == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap << 1 : 16;
        uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
}

/* Return divisor count d(n) using the current primes list */
static uint64_t divisor_count(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t m = n;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > m) break;
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        tau *= (exp + 1);
    }
    if (m > 1) {
        /* m is prime (the first prime larger than all stored in 'primes') */
        tau *= 2;
        add_prime((uint32_t)m);
    }
    return tau;
}

/* Dynamic array of sequence values (seq_t type) */
static seq_t *seq = NULL;
static size_t seq_len = 0, seq_cap = 0;
static uint64_t processed = 0;  /* last integer examined */

static void push_seq(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 8;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        seq = tmp;
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* Main function to return the n-th term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined, but guard */

    /* Grow the sequence until we have at least n terms */
    while (seq_len < (size_t)n) {
        ++processed;                              /* examine the next integer */
        uint64_t tau = divisor_count(processed);  /* number of divisors */

        if (is_triangular(tau))
            push_seq((seq_t)processed);            /* use cast to seq_t */
    }
    return seq[n - 1];
}
