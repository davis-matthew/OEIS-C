/*
 * A044432  a(n) is the number whose base-2 representation is d(0)d(1)...d(n),
 *          where d=A005614 (the infinite Fibonacci word).
 * https://oeis.org/A044432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*a(n-1) + d(n) for n > 0, a(0) = 1 where d(n) is the nth
 *            bit of the Fibonacci word A005614
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------ */
/*  Cache for computed values of a(n)                            */
/* ------------------------------------------------------------ */
static seq_t *cache = NULL;               /* array of computed a(n)          */
static idx_t  cache_len = 0;              /* number of entries currently used */

/* ------------------------------------------------------------ */
/*  Fibonacci word lengths L[k] = |S_k| where
   S_0 = "1",  S_1 = "10",  S_k = S_{k-1} S_{k-2}.  */
static idx_t L[64];
static int   L_size = 0;

/* Ensure that L array contains lengths up to at least 'limit'+1.  */
static void ensure_lenL(idx_t limit)
{
    if (L_size == 0) {
        L[0] = 1;
        L_size = 1;
    }
    while (L[L_size-1] <= limit) {
        if (L_size == 1) {
            L[1] = 2;
        } else {
            L[L_size] = L[L_size-1] + L[L_size-2];
        }
        L_size++;
    }
}

/* Return bit d(n) of the Fibonacci word A005614.                  */
static idx_t fib_word_bit(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 0;

    ensure_lenL(n);

    /* find the smallest k with L[k] > n */
    int k = 1;
    while (L[k] <= n) k++;

    /* iterative descent on the recursion S_k = S_{k-1} S_{k-2} */
    while (n >= 2) {
        if (n < L[k-1]) {
            k = k - 1;
        } else {
            n -= L[k-1];
            k = k - 2;
        }
    }
    return (n == 0) ? 1 : 0;
}

/* Ensure cache array has room for indices 0..n.                    */
static void ensure_cache_size(idx_t n)
{
    if (cache_len <= n) {
        size_t new_cap = cache_len ? cache_len : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return;            /* allocation failure – caller will fail */
        cache = new_cache;
    }
}

/* ------------------------------------------------------------ */
/*  Public sequence function                                    */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Initialize cache on first call */
    if (cache == NULL) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 1);
        if (!cache) return (seq_t)0;            /* allocation failure */
        cache[0] = (seq_t)1;                    /* a(0) = 1 */
        cache_len = 1;
    }

    /* Compute a(n) if not already present */
    if (n >= cache_len) {
        ensure_cache_size(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            idx_t bit = fib_word_bit(i);
            cache[i] = (cache[i-1] << 1) + (seq_t)bit;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
