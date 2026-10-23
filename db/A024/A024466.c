/*
 * A024466  a(n) = s(1)*t(n) + s(2)*t(n-1) + ... + s(k)*t(n+1-k), where k =
 *          floor((n+1)/2), s = (Fibonacci numbers), t = A023533.
 * https://oeis.org/A024466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor((n+1)/2)} Fibonacci(k)*A023533(n+1-k)
 * Range:     u8 n <= 32, u16 n <= 58, u32 n <= 102, u64 n <= 212,
 *            u128 n <= 405, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A023533(m) = 1 if m is a tetrahedral number, otherwise 0. */

#include <stdlib.h>

/* Static cache for Fibonacci numbers: fib_cache[i] = F(i),  i ≥ 0. */
static seq_t *fib_cache = NULL;
static idx_t fib_len = 0;  /* number of elements currently allocated (max index + 1). */

/* Ensure fib_cache has Fibonacci numbers up to index max_k inclusive. */
static void ensure_fib(idx_t max_k)
{
    if (fib_len > max_k) return;          /* cache already sufficient */

    idx_t new_len = max_k + 1;            /* number of elements needed */
    if (fib_cache == NULL) {
        fib_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (!fib_cache) return;           /* allocation failed; ignore */
    } else {
        seq_t *tmp = (seq_t*)realloc(fib_cache, new_len * sizeof(seq_t));
        if (!tmp) return;                 /* allocation failed; ignore */
        fib_cache = tmp;
    }

    /* Initialise is required only on first allocation */
    if (fib_len == 0) {
        fib_cache[0] = 0;
        if (new_len > 1) fib_cache[1] = 1;
        fib_len = (new_len >= 2) ? 2 : 1;
    }

    /* Compute new Fibonacci values */
    for (idx_t i = fib_len; i <= max_k; ++i) {
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_len = new_len;
}

/* Compute the n-th term of sequence A024466.  Offset 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* offset is 1; below offset undefined */

    /* Required Fibonacci indices go up to floor((n+1)/2) */
    idx_t max_fib = (n + 1) / 2;
    ensure_fib(max_fib);

    seq_t sum = 0;

    /* Lower bound for tetrahedral numbers to contribute */
    idx_t t_min = (n + 2) / 2;                  /* ceil((n+1)/2) */

    /* Iterate over tetrahedral numbers m*(m+1)*(m+2)/6 ≤ n */
    for (idx_t m = 1;; ++m) {
        unsigned __int128 prod = (unsigned __int128)m * (m + 1) * (m + 2);
        idx_t tval = (idx_t)(prod / 6);
        if (tval > n) break;                  /* beyond needed range */

        if (tval >= t_min) {
            idx_t k = n + 1 - tval;           /* corresponding Fibonacci index */
            sum += fib_cache[k];
        }
    }

    return sum;
}
