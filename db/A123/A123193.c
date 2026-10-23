/*
 * A123193  Natural numbers with number of divisors equal to a Fibonacci
 *          number.
 * https://oeis.org/A123193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th natural number whose number of divisors is a
 *            Fibonacci number
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Linear time sieve‑style generation, memoised across calls.) */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *terms = NULL;          /* array of found terms */
static size_t terms_len = 0;         /* number of terms stored */
static size_t terms_cap = 0;         /* current capacity of array */
static idx_t   candidate = 1;        /* next natural number to test */

/* Fibonacci table (enough for divisor counts up to >10^5) */
static uint32_t fibs[200];
static size_t fibs_cnt = 0;

/* initialise Fibonacci table */
static void init_fibs(void)
{
    if (fibs_cnt != 0) return;        /* already initialised */
    uint32_t a = 0, b = 1;
    fibs[fibs_cnt++] = a;
    fibs[fibs_cnt++] = b;
    while (1) {
        uint64_t c = (uint64_t)a + b;
        if (c > 1000000u) break;     /* sufficient upper bound */
        fibs[fibs_cnt++] = (uint32_t)c;
        a = b;
        b = (uint32_t)c;
    }
}

/* check whether x is a Fibonacci number */
static bool is_fib_uint32(uint32_t x)
{
    for (size_t i = 0; i < fibs_cnt; ++i) {
        if (fibs[i] == x) return true;
        if (fibs[i] > x) break;
    }
    return false;
}

/* divisor count for numbers up to 64 bits (trial division) */
static uint32_t divisor_count(idx_t n)
{
    uint64_t m = n;
    uint32_t d = 1;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            d *= (e + 1);
        }
    }
    if (m > 1) d *= 2;
    return d;
}

/* grow terms array to at least wanted capacity */
static void grow_terms_size(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap == 0 ? 16 : terms_cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) exit(EXIT_FAILURE); /* allocation failure */
    terms = new_arr;
    terms_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe return */

    /* initialise once */
    if (terms_cap == 0) {
        init_fibs();
        grow_terms_size(16);
    }

    /* extend the sequence until we have n terms */
    while (terms_len < n) {
        uint32_t d = divisor_count(candidate);
        if (is_fib_uint32(d)) {
            grow_terms_size(terms_len + 1);
            terms[terms_len++] = (seq_t)candidate;
        }
        ++candidate;
    }

    return terms[n - 1];
}
