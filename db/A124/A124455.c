/*
 * A124455  Numbers n such that 24n divides the sum of the first 24n nonzero
 *          Fibonacci numbers.
 * https://oeis.org/A124455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer m such that 24*m divides the sum
 *            of the first 24*m non‑zero Fibonacci numbers. Equivalently,
 *            F_{24m+2} ≡ 1 (mod 24m)
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Fast doubling: returns (F_n, F_{n+1}) modulo mod. */
static void fib_pair(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, mod, &a, &b);          /* (F_k, F_{k+1}) */

    /* compute F_{2k} and F_{2k+1}   */
    seq_t two_b        = (b * 2) % mod;
    seq_t two_b_minus_a = (two_b + mod - a) % mod;
    seq_t c = (a * two_b_minus_a) % mod;                     /* F_{2k}   */
    seq_t d = ((a * a) % mod + (b * b) % mod) % mod;          /* F_{2k+1} */

    if (n & 1) {
        *fn  = d;                                   /* F_{2k+1} */
        *fn1 = (c + d) % mod;                       /* F_{2k+2} */
    } else {
        *fn  = c;                                   /* F_{2k}   */
        *fn1 = d;                                   /* F_{2k+1} */
    }
}

/* Compute F_k modulo mod. */
static seq_t fib_mod(idx_t k, seq_t mod)
{
    seq_t fn, fn1;
    fib_pair(k, mod, &fn, &fn1);
    return fn;
}

/* Cache of sequence values. */
static idx_t cache_last_checked = 0;   /* last integer examined          */
static seq_t *cache_vals = NULL;       /* array of found terms            */
static size_t cache_size   = 0;        /* number of terms stored          */
static size_t cache_cap    = 0;        /* allocated capacity              */

/* Grow the cache array, doubling its capacity. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *new_ptr = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();          /* out of memory */
    cache_vals = new_ptr;
    cache_cap  = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Generate terms until we have nth term. */
    while (cache_size < n) {
        /* Test the next candidate integer. */
        ++cache_last_checked;
        idx_t cand = cache_last_checked;

        seq_t mod = (seq_t)cand * 24ULL;                 /* 24*m */
        seq_t fval = fib_mod(24ULL * cand + 2ULL, mod);

        if (fval == 1) {                /* condition holds  */
            if (cache_size == cache_cap)
                grow_cache();
            cache_vals[cache_size++] = (seq_t)cand;   /* store as seq_t */
        }
    }
    return cache_vals[n - 1];
}
