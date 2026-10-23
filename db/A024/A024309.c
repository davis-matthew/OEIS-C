/*
 * A024309  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = (natural numbers >= 2), t = (Fibonacci
 *          numbers).
 * https://oeis.org/A024309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A024309: a(n) = Σ_{j=2..⌊(n+3)/2⌋} j * Fibonacci(n-j+2)   */
/* (Offset 1)  Fibonacci indices are computed so that the largest needed  */
/* is Fibonacci(n), hence an incremental Fibonacci cache is maintained. */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t *fib_cache = NULL;  /* caches Fibonacci numbers 0..N */
static idx_t  fib_cache_max = 0; /* maximum index currently cached */

static void ensure_fib_cache(idx_t n)
{
    if (fib_cache == NULL) {
        /* initialize cache with F0=0, F1=1 */
        fib_cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (fib_cache == NULL) abort();
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_cache_max = 1;
    }
    if (n <= fib_cache_max) return;       /* already cached */

    /* grow cache to accommodate indices up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(fib_cache, new_size * sizeof(seq_t));
    if (new_cache == NULL) abort();
    fib_cache = new_cache;

    /* compute new Fibonacci values */
    for (idx_t i = fib_cache_max + 1; i <= n; ++i) {
        /* F(i) = F(i-1) + F(i-2) */
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    }
    fib_cache_max = n;
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be ≥ 1 by the caller (per OEIS convention) */
    ensure_fib_cache(n);

    idx_t k = (n + 3) / 2;          /* upper index in the sum */
    seq_t sum = (seq_t)0;

    for (idx_t j = 2; j <= k; ++j) {
        idx_t fib_index = n - j + 2;   /* Fibonacci index needed */
        sum += (seq_t)j * fib_cache[fib_index];
    }
    return sum;
}
