/*
 * A154147  Indices k such that 19 plus the k-th triangular number is a
 *          perfect square.
 * https://oeis.org/A154147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154147: indices k such that 19 + k*(k+1)/2 is a perfect square.
   Recurrence: a(n) = a(n-1) + 6*a(n-2) - 6*a(n-3) - a(n-4) + a(n-5).
   Initial values: a(1)=3, a(2)=9, a(3)=30, a(4)=60, a(5)=179. */
#include <stdlib.h>

/* Static cache for memoization (1‑based indexing). */
static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;   /* number of elements allocated in seq_cache */
static size_t computed   = 0;  /* largest index that has already been computed */

/* Ensure the cache can hold indices up to n (inclusive).  */
static void ensure_cache(idx_t n)
{
    if (n == 0 || n < cache_cap) return;

    size_t new_cap = (cache_cap == 0 ? 8 : cache_cap << 1);
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_arr = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);                 /* allocation failed */
    seq_cache = new_arr;
    cache_cap = new_cap;
}

/* Compute the n‑th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* undefined for n < 1 */

    /* Return cached value if already computed. */
    if (n <= computed) return seq_cache[(size_t)n];

    /* Compute missing terms up to n. */
    ensure_cache(n);
    for (size_t i = computed + 1; i <= (size_t)n; ++i) {
        if (i == 1) seq_cache[i] = (seq_t)3;
        else if (i == 2) seq_cache[i] = (seq_t)9;
        else if (i == 3) seq_cache[i] = (seq_t)30;
        else if (i == 4) seq_cache[i] = (seq_t)60;
        else if (i == 5) seq_cache[i] = (seq_t)179;
        else {
            seq_t a1 = seq_cache[i-1];
            seq_t a2 = seq_cache[i-2];
            seq_t a3 = seq_cache[i-3];
            seq_t a4 = seq_cache[i-4];
            seq_t a5 = seq_cache[i-5];
            seq_t val = a1 + 6*a2 - 6*a3 - a4 + a5;
            seq_cache[i] = val;
        }
    }
    computed = (size_t)n;
    return seq_cache[(size_t)n];
}
