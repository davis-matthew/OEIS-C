/*
 * A192474  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x)=1+x^(n+1)+x^(2n).
 * https://oeis.org/A192474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + F_n + F_{2*n-1} where F_k is the k‑th Fibonacci
 *            number (F_0=0, F_1=1). This gives the constant term of the
 *            reduction of p(n,x)=1+x^(n+1)+x^(2n) under the substitution x^2
 *            → x+1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- Fibonacci cache ---------- */
static seq_t *fib_cache = NULL;
static size_t fib_cache_size = 0;

/* Ensure that fib_cache contains Fibonacci numbers up to index k (inclusive). */
static void ensure_fib(idx_t k)
{
    if (k < fib_cache_size) return;      /* already computed */

    size_t new_size = (size_t)k + 1;      /* need indices 0..k */
    seq_t *new_cache = (seq_t*)realloc(fib_cache, new_size * sizeof(seq_t));
    if (!new_cache) return;              /* allocation failure – silently ignore */

    if (fib_cache_size == 0) {
        new_cache[0] = 0;                /* F_0 */
        if (new_size > 1) new_cache[1] = 1;   /* F_1 */
        for (idx_t i = 2; i < new_size; ++i)
            new_cache[i] = new_cache[i-1] + new_cache[i-2];
    } else {
        for (idx_t i = fib_cache_size; i < new_size; ++i)
            new_cache[i] = new_cache[i-1] + new_cache[i-2];
    }

    fib_cache = new_cache;
    fib_cache_size = new_size;
}

/* ------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* out of OEIS range, safe default */

    idx_t need = 2 * n - 1;
    if (need < n) need = n;             /* the larger index is needed */

    ensure_fib(need);

    seq_t Fn    = fib_cache[n];
    seq_t F2n1  = fib_cache[need];

    return 1 + Fn + F2n1;               /* 1 is an integer literal, OK in C & C++ */
}
