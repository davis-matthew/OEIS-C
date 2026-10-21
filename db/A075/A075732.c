/*
 * A075732  Fibonacci numbers F(k) for k not squarefree (A013929).
 * https://oeis.org/A075732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(k) where k is the nth non‑squarefree integer
 *            (0‑based). A non‑squarefree integer is one divisible by a square
 *            > 1. a(0) = Fibonacci(4), a(1) = Fibonacci(8), …
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   is_not_squarefree_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_not_squarefree_uint64.h"

/* ------- Fast doubling Fibonacci (exact arithmetic) ------- */
static void fib_pair(idx_t n, seq_t *F, seq_t *Fnext)
{
    if (n == 0) {
        *F     = (seq_t)0;
        *Fnext = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);                 /* a = F(k), b = F(k+1)  */
    seq_t two_b = b + b;                      /* 2*F(k+1)              */
    seq_t c     = a * (two_b - a);            /* F(2k)                  */
    seq_t d     = a * a + b * b;              /* F(2k+1)                */
    if (n & 1) {
        *F     = d;
        *Fnext = c + d;
    } else {
        *F     = c;
        *Fnext = d;
    }
}
static seq_t fibonacci(idx_t n)
{
    seq_t f, fn1;
    fib_pair(n, &f, &fn1);
    return f;
}

/* ------- Cache of non‑squarefree integers ------- */
static uint64_t *nf_cache     = NULL;
static size_t    nf_cache_size = 0;
static size_t    nf_cache_used = 0;
static uint64_t  last_checked  = 0;

/* Grow the cache when the allocation becomes full */
static void grow_cache(void)
{
    size_t new_size = nf_cache_size ? nf_cache_size * 2 : 1024;
    uint64_t *tmp = (uint64_t *)realloc(nf_cache, new_size * sizeof(uint64_t));
    if (!tmp) abort();          /* fatal error: allocation failed */
    nf_cache     = tmp;
    nf_cache_size = new_size;
}

/* Return the `index`-th non‑squarefree integer (0‑based). */
static uint64_t nth_non_squarefree(idx_t index)
{
    while (nf_cache_used <= (size_t)index) {
        ++last_checked;
        if (is_not_squarefree_uint64(last_checked)) {
            if (nf_cache_used == nf_cache_size)
                grow_cache();
            nf_cache[nf_cache_used++] = last_checked;
        }
    }
    return nf_cache[(size_t)index];
}

/* ------- Public function required by the harness ------- */
seq_t a(idx_t n)
{
    uint64_t k = nth_non_squarefree(n);
    return fibonacci((idx_t)k);
}
