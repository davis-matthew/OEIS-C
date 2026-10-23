/*
 * A101907  Numbers k such that the arithmetic mean of the first k+1 Fibonacci
 *          numbers (beginning with F(0)) is an integer.
 * https://oeis.org/A101907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = k such that (F(k+2)-1) mod (k+1) == 0 (i.e. the
 *            arithmetic mean of the first k+1 Fibonacci numbers is an
 *            integer)
 * Range:     u8 n <= 50, u16 n <= 4921, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Fast‑doubling Fibonacci modulo.  Returns F(n) mod mod.  */
static uint64_t fib_mod_uint64(uint64_t n, uint64_t mod)
{
    if (mod == 1) return 0;
    if (n == 0) return 0;
    uint64_t a = 0;          /* F(0) */
    uint64_t b = 1;          /* F(1) */
    int top = 63 - __builtin_clzll(n);
    for (int i = top; i >= 0; --i) {
        /* doubling formulas: c = F(2k), d = F(2k+1) */
        uint64_t t1 = (2 * b) % mod;
        if (a > t1) t1 += mod;
        t1 -= a;                          /* (2*b - a) mod mod */
        uint64_t c = (a * t1) % mod;      /* F(2k) */
        uint64_t d = ((a * a) % mod + (b * b) % mod) % mod; /* F(2k+1) */
        if ((n >> i) & 1) {
            a = d;
            b = (c + d) % mod;
        } else {
            a = c;
            b = d;
        }
    }
    return a;
}

/* Dynamic array to cache the terms of the sequence. */
static seq_t *terms   = NULL;
static idx_t term_count = 0;
static idx_t next_k    = 0;      /* next k to test (0‑based) */
static uint64_t terms_cap = 0;

static void ensure_terms_cap(size_t min_cap)
{
    if (terms_cap < min_cap) {
        uint64_t new_cap = terms_cap ? terms_cap * 2 : 16;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_terms) abort();
        terms = new_terms;
        terms_cap = new_cap;
    }
}

/* Return the n‑th term of the sequence (n ≥ 1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined by OEIS spec; guard against underflow */

    /* Generate terms until we have at least n of them. */
    while (term_count < n) {
        uint64_t k      = next_k++;
        uint64_t mod    = k + 1ULL;
        uint64_t f      = fib_mod_uint64(k + 2ULL, mod);   /* F(k+2) mod (k+1) */
        if (mod == 1 || f == 1) {        /* average is integer */
            ensure_terms_cap(term_count + 1);
            terms[term_count++] = (seq_t)k;   /* store qualifying k */
        }
    }
    return terms[n - 1];
}
