/*
 * A123741  A second version of Fibonacci factorials besides A003266.
 * https://oeis.org/A123741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{j=1..n} (F(n+2) - F(j+1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> fib_vec;
#else
static seq_t *fib_cache = NULL;
static size_t fib_cap   = 0;
static idx_t fib_len    = 0;
#endif

/* Ensure Fibonacci values up to index n (inclusive) are cached. */
static void ensure_fib(idx_t n)
{
#ifdef __cplusplus
    /* C++: use std::vector. */
    size_t old_size = fib_vec.size();
    if (old_size > (size_t)n) return;
    fib_vec.resize((size_t)n + 1);
    if (old_size == 0) {
        fib_vec[0] = (seq_t)0;
        if (fib_vec.size() > 1) fib_vec[1] = (seq_t)1;
    } else if (old_size == 1) {
        fib_vec[1] = (seq_t)1;
    }
    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 2) continue;
        fib_vec[i] = fib_vec[i-1] + fib_vec[i-2];
    }
#else
    /* C: use malloc/realloc. */
    if (fib_len > n) return;
    size_t new_cap = (fib_cap > (size_t)n + 1) ? fib_cap : ((size_t)n + 1);
    seq_t *tmp = (seq_t*)realloc(fib_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    fib_cache = tmp;
    fib_cap   = new_cap;

    if (fib_len == 0) {
        fib_cache[0] = (seq_t)0;
        if (fib_cap > 1) fib_cache[1] = (seq_t)1;
        fib_len = 2;
    }
    for (idx_t i = fib_len; i <= n; ++i) {
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_len = n + 1;
#endif
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1)=1.  Undefined for n=0, return 0 safely. */
    if (n == 0) return (seq_t)0;

    /* Compute Fibonacci up to n+2. */
    ensure_fib(n + 2);

#ifdef __cplusplus
    seq_t fn = fib_vec[(size_t)n + 2];
    seq_t result = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t term = fn - fib_vec[(size_t)j + 1];
        result *= term;
    }
    return result;
#else
    seq_t fn = fib_cache[(size_t)n + 2];
    seq_t result = (seq_t)1;
    for (idx_t j = 1; j <= n; ++j) {
        seq_t term = fn - fib_cache[(size_t)j + 1];
        result *= term;
    }
    return result;
#endif
}
