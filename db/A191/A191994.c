/*
 * A191994  (Sum of first n Fibonacci numbers) times (product of first n
 *          Fibonacci numbers).
 * https://oeis.org/A191994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (F(n+2)-1) * product_{k=1..n} F(k) where Fib(1)=1,
 *            Fib(2)=1, Fib(3)=2,
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fib_cache = NULL;   /* Fibonacci numbers, indexed by n (1‑based) */
static size_t fib_cap = 0;        /* allocated capacity of fib_cache   */
static idx_t   fib_max_cached = 0;/* largest n for which fib_cache[n] is valid */

static seq_t *prod_cache = NULL;  /* cumulative products, prod_cache[n] = Π_{k=1..n}F(k) */
static size_t prod_cap = 0;
static idx_t   prod_max_cached = 0;

/* Allocate/extend fib_cache to hold indices up to 'needed' */
static void ensure_fib_capacity(idx_t needed)
{
    if (needed < (idx_t)fib_cap) return;
    size_t new_cap = fib_cap ? fib_cap * 2 : (size_t)needed + 5;
    if (new_cap <= (size_t)needed) new_cap = (size_t)needed + 5;

    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) return;                 /* allocation failed – emit 0 for safety */

    if (fib_cache) {
        for (size_t i = 0; i < fib_cap; ++i) new_arr[i] = fib_cache[i];
        free(fib_cache);
    }
    for (size_t i = fib_cap; i < new_cap; ++i) new_arr[i] = 0;
    fib_cache = new_arr;
    fib_cap   = new_cap;
}

/* Allocate/extend prod_cache to hold indices up to 'needed' */
static void ensure_prod_capacity(idx_t needed)
{
    if (needed < (idx_t)prod_cap) return;
    size_t new_cap = prod_cap ? prod_cap * 2 : (size_t)needed + 5;
    if (new_cap <= (size_t)needed) new_cap = (size_t)needed + 5;

    seq_t *new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) return;

    if (prod_cache) {
        for (size_t i = 0; i < prod_cap; ++i) new_arr[i] = prod_cache[i];
        free(prod_cache);
    }
    for (size_t i = prod_cap; i < new_cap; ++i) new_arr[i] = 0;
    prod_cache = new_arr;
    prod_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0 */

    /* We need Fib(n+2) and the product up to Fib(n) */
    idx_t need_fib = n + 2;
    idx_t need_prod = n;

    /* Ensure caches are large enough */
    ensure_fib_capacity(need_fib);
    ensure_prod_capacity(need_prod);

    /* Compute missing Fibonacci numbers */
    if (fib_max_cached < need_fib) {
        if (fib_max_cached == 0) {      /* initialise base case */
            fib_cache[1] = (seq_t)1;
            fib_cache[2] = (seq_t)1;
            fib_max_cached = 2;
        }
        for (size_t i = (size_t)(fib_max_cached + 1); i <= (size_t)need_fib; ++i) {
            if (i <= 2) continue;       /* base already set */
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
        }
        fib_max_cached = need_fib;
    }

    /* Compute missing cumulative products */
    if (prod_max_cached < need_prod) {
        if (prod_max_cached == 0) {      /* product of zero numbers = 1 */
            prod_cache[0] = (seq_t)1;
            prod_max_cached = 0;
        }
        for (size_t i = (size_t)(prod_max_cached + 1); i <= (size_t)need_prod; ++i) {
            prod_cache[i] = prod_cache[i-1] * fib_cache[i];
        }
        prod_max_cached = need_prod;
    }

    /* a(n) = (Fib(n+2) - 1) * product_{k=1..n} Fib(k) */
    seq_t term = fib_cache[n+2] - (seq_t)1;
    term = term * prod_cache[n];
    return term;
}
