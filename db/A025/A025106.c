/*
 * A025106  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n-k+1), where k = [
 *          n/2 ], s = (F(2), F(3), F(4), ...), t = (odd natural numbers).
 * https://oeis.org/A025106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{i=1..ceil(n/2)} F_{i+1} * (2*(n - i + 1) + 1) where
 *            F_k is the k‑th Fibonacci number with F_0 = 0, F_1 = 1 and
 *            ceil(n/2) = (n+1)/2 using integer division
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static cache for Fibonacci numbers */
static seq_t *fib_cache = NULL;
static size_t fib_cap = 0;
static size_t fib_len = 0;   /* number of Fibonacci values stored (fib_cache[0]..fib_cache[fib_len-1]) */

/* ensure that fib_cache contains Fibonacci numbers up to index 'need' (inclusive) */
static void ensure_fib(size_t need)
{
    if (need < fib_len)
        return;

    /* grow the cache */
    size_t new_cap = fib_cap ? fib_cap * 2 : 8;
    while (new_cap <= need)
        new_cap *= 2;

    seq_t *new_fib = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_fib)
        abort();   /* allocation failed */

    if (fib_cache) {
        memcpy(new_fib, fib_cache, fib_len * sizeof(seq_t));
        free(fib_cache);
    }
    fib_cache = new_fib;
    fib_cap = new_cap;

    /* initialise the first two Fibonacci numbers */
    if (fib_len == 0) {
        fib_cache[0] = 0;
        fib_cache[1] = 1;
        fib_len = 2;
    }

    /* compute missing Fibonacci numbers */
    while (fib_len <= need) {
        fib_cache[fib_len] = fib_cache[fib_len - 1] + fib_cache[fib_len - 2];
        ++fib_len;
    }
}

/* sequence function */
seq_t a(idx_t n)
{
    /* ceil(n/2) = (n + 1) / 2 */
    idx_t k = (n + 1) / 2;
    /* we need Fibonacci numbers up to index k+1 (since s(i)=F_{i+1}) */
    ensure_fib((size_t)(k + 1));

    seq_t sum = 0;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t s = fib_cache[(size_t)(i + 1)];            /* F_{i+1} */
        seq_t t = (((seq_t)n - (seq_t)i + 1) * 2) + 1;   /* 2*(n - i + 1) + 1 */
        sum += s * t;
    }
    return sum;
}
