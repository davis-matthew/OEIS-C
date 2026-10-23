/*
 * A034734  Dirichlet convolution of b_n=2^(n-1) with Fibonacci numbers.
 * https://oeis.org/A034734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(d-1) * Fib(n/d) (Fib(1)=1, Fib(2)=1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static tables for pre‑computed values, shared by all calls */
static seq_t *fib   = NULL;  /* Fibonacci numbers, 1‑indexed */
static seq_t *pow2  = NULL;  /* 2^(d-1), 1‑indexed */
static seq_t *cache = NULL;  /* memoised a(n), 0 indicates “not yet computed” */
static idx_t  cap  = 0;      /* current maximum index for which tables are allocated */

/* enlarge tables to cover at least n, keeping previous data */
static void ensure_precomp(idx_t n)
{
    if (n <= cap) return;          /* already covered */

    idx_t new_cap = n;
    seq_t *new_fib   = (seq_t*)malloc(sizeof(seq_t) * (new_cap + 1));
    seq_t *new_pow2  = (seq_t*)malloc(sizeof(seq_t) * (new_cap + 1));
    seq_t *new_cache = (seq_t*)malloc(sizeof(seq_t) * (new_cap + 1));

    /* copy existing data, if any */
    if (fib)   memcpy(new_fib,   fib,   sizeof(seq_t)*(cap + 1));
    if (pow2)  memcpy(new_pow2,  pow2,  sizeof(seq_t)*(cap + 1));
    if (cache) memcpy(new_cache, cache, sizeof(seq_t)*(cap + 1));

    /* initialise entries which were not previously allocated */
    if (new_cap >= 0) {
        new_fib[0]   = (seq_t)0;
        new_pow2[0]  = (seq_t)0;
        new_cache[0] = (seq_t)0;
    }
    if (new_cap >= 1) { new_fib[1] = (seq_t)1;   new_pow2[1] = (seq_t)1; }
    if (new_cap >= 2) { new_fib[2] = (seq_t)1;   new_pow2[2] = (seq_t)2; }

    /* grow Fibonacci numbers and powers of two */
    idx_t i_start = (cap > 2) ? cap + 1 : 3;
    for (idx_t i = i_start; i <= new_cap; ++i) {
        new_fib[i]  = new_fib[i-1] + new_fib[i-2];
        new_pow2[i] = new_pow2[i-1] * (seq_t)2;
    }

    /* clear newly allocated cache entries */
    for (idx_t i = cap + 1; i <= new_cap; ++i) new_cache[i] = (seq_t)0;

    /* replace old tables */
    free(fib);   free(pow2);   free(cache);
    fib    = new_fib;
    pow2   = new_pow2;
    cache  = new_cache;
    cap    = new_cap;
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not defined for n < 1 */
    ensure_precomp(n);

    if (cache[n] != (seq_t)0) return cache[n];  /* already computed */

    seq_t total = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            seq_t t1 = pow2[d1] * fib[d2];
            total += t1;
            if (d1 != d2) {
                seq_t t2 = pow2[d2] * fib[d1];
                total += t2;
            }
        }
    }
    cache[n] = total;
    return total;
}
