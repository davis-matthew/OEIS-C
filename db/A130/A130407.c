/*
 * A130407  A diagonal of triangle A130405.
 * https://oeis.org/A130407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n+1)*a(n-1) + F(n+2)*A003266(n) where F(k) =
 *            Fibonacci(k) (F(0)=0, F(1)=1) and A003266(n) = product of the
 *            first n non‑zero Fibonacci numbers (A003266(0)=1). a(0)=1. The
 *            implementation uses static caches
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   seq_alloc, seq_one, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"   /* seq_alloc, seq_free */
#include "../utils/seq_realloc.h" /* seq_realloc */
#include "../utils/seq_one.h"     /* seq_one() */

static seq_t *fib     = NULL; /* Fibonacci numbers, indices 0‥ */
static seq_t *prod    = NULL; /* A003266 values, indices 0‥ */
static seq_t *a_cache = NULL; /* a(n) values, indices 0‥ */

static size_t fib_len  = 0; /* current size of fib array (number of elements) */
static size_t prod_len = 0; /* current size of prod array */
static size_t a_len    = 0; /* current size of a array */

/* Ensure that all required cache entries up to index n are computed. */
static void ensure_cache(idx_t n)
{
    /* ---- Fibonacci cache ---- */
    size_t need_fib = (size_t)n + 3;   /* need indices 0 … n+2 inclusive */
    if (fib_len < need_fib) {
        size_t prev = fib_len;
        fib_len = need_fib;
        fib = seq_realloc(fib, prev, fib_len);

        /* initialise missing low indices if the array was empty */
        if (prev == 0) {
            fib[0] = 0;
            fib[1] = 1;
            fib[2] = 1;
            prev = 3;
        } else if (prev == 1) {
            fib[1] = 1;
            if (need_fib >= 3) fib[2] = 1;
            prev = 3;
        } else if (prev == 2) {
            fib[2] = 1;
            prev = 3;
        }

        /* compute the rest */
        for (idx_t i = (idx_t)prev; i < fib_len; ++i)
            fib[i] = fib[i-1] + fib[i-2];
    }

    /* ---- Product (A003266) cache ---- */
    size_t need_prod = (size_t)n + 1;   /* need indices 0 … n inclusive */
    if (prod_len < need_prod) {
        size_t prev = prod_len;
        prod_len = need_prod;
        prod = seq_realloc(prod, prev, prod_len);

        /* initialise if the array was empty */
        if (prev == 0) {
            prod[0] = seq_one();          /* empty product */
            prev = 1;
        }

        for (idx_t i = (idx_t)prev; i < prod_len; ++i)
            prod[i] = prod[i-1] * fib[i];
    }

    /* ---- a(n) cache ---- */
    size_t need_a = (size_t)n + 1;   /* need indices 0 … n inclusive */
    if (a_len < need_a) {
        size_t prev = a_len;
        a_len = need_a;
        a_cache = seq_realloc(a_cache, prev, a_len);

        if (prev == 0) {         /* have only a(0) so far */
            a_cache[0] = seq_one();  /* a(0) = 1 */
            prev = 1;
        }

        for (idx_t i = (idx_t)prev; i < a_len; ++i) {
            /* a(i) = F(i+1)*a(i-1) + F(i+2)*prod[i] */
            seq_t term1 = fib[i+1] * a_cache[i-1];
            seq_t term2 = fib[i+2] * prod[i];
            a_cache[i] = term1 + term2;
        }
    }
}

/* Sequence entry function */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return a_cache[n];
}
