/*
 * A050612  Sum_{i=0..n} (C(n,i) mod 2)*Fibonacci(2i+3) =
 *          FL(n+3)Product(L(2^i)^bit(n,i),i=0..).
 * https://oeis.org/A050612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i=0..n} (C(n,i) mod 2) · Fib(2i+3) (Fibonacci numbers
 *            are taken with F(0)=0, F(1)=1). The sum is taken over all
 *            sub‑masks i of n (i.e. i & ~n == 0)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cache for Fibonacci numbers.  fib[0] = 0, fib[1] = 1. */
static seq_t *fib_cache   = NULL;  /* array of Fibonacci values   */
static idx_t  fib_len     = 0;     /* number of values currently stored (highest index + 1) */
static size_t fib_cap     = 0;     /* allocated capacity (bytes)   */

/* Ensure that fib_cache contains Fibonacci values up to at least index max_idx. */
static void
ensure_fib(idx_t max_idx)
{
    if (fib_len > max_idx)
        return;

    /* If nothing is allocated yet, reserve a small initial capacity. */
    if (!fib_cache) {
        fib_cap = 8;
        fib_cache = (seq_t *)malloc(sizeof(seq_t) * fib_cap);
        if (!fib_cache) exit(1);
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_len     = 2;
    }

    /* Grow the array if necessary. */
    while (fib_len <= max_idx && fib_len > fib_cap) {
        size_t new_cap = fib_cap * 2;
        seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fib_cache = tmp;
        fib_cap   = new_cap;
    }

    /* Re‑allocate if the current capacity is insufficient. */
    if (fib_len <= max_idx && fib_cap < (size_t)(max_idx + 1)) {
        size_t new_cap = 2 * fib_cap;
        if (new_cap < (size_t)(max_idx + 1))
            new_cap = max_idx + 1;
        seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        fib_cache = tmp;
        fib_cap   = new_cap;
    }

    /* Compute any missing Fibonacci values up to max_idx. */
    for (idx_t i = fib_len; i <= max_idx; ++i) {
        fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
    }
    fib_len = max_idx + 1;
}

/* Main sequence function */
extern seq_t
a(idx_t n)
{
    /* We need Fibonacci numbers up to index 2*n+3. */
    ensure_fib(2 * n + 3);

    seq_t result = (seq_t)0;
    idx_t subset = n;

    /* Enumerate all sub‑masks of n (including 0 and n) using the
       efficient bit trick.  For each sub‑mask i, add Fib(2*i+3). */
    for (;;) {
        idx_t fib_index = 2 * subset + 3;
        result += fib_cache[fib_index];
        if (subset == 0)
            break;
        subset = (subset - 1) & n;
    }
    return result;
}
