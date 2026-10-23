/*
 * A120185  a(1)=9; a(n)=floor((55+sum(a(1) to a(n-1)))/6).
 * https://oeis.org/A120185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A120185:
 * a(1) = 9
 * a(n) = floor((55 + sum_{k=1}^{n-1} a(k)) / 6)      for n > 1
 *
 * The implementation uses a dynamic, memoizing array so that
 * a(n) can be returned in O(1) after an O(n) precomputation is needed.
 * It works for any sequence value type seq_t and index type idx_t.
 */
#include <stdlib.h>
#include <string.h> /* for memcpy */

/* Static cache for computed terms */
static seq_t *a_cache   = NULL;   /* a[n] */
static seq_t *s_cache   = NULL;   /* cumulative sum s[n] = a[1]+...+a[n] */
static idx_t  num_computed = 0;   /* highest n that has been computed */

/* Ensure the caches have room for indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (a_cache == NULL) {
        /* First allocation: size at least 2 to store a[0] (unused) and a[1] */
        idx_t first_size = n > 1 ? n : 1;
        size_t sz = (size_t)(first_size + 1);
        a_cache = (seq_t *)malloc(sz * sizeof(seq_t));
        s_cache = (seq_t *)malloc(sz * sizeof(seq_t));
        /* initialise index 0 to 0 for cumulative sums */
        if (a_cache) a_cache[0] = (seq_t)0;
        if (s_cache) s_cache[0] = (seq_t)0;
        return;
    }

    if (n <= num_computed) return;          /* already enough space */

    size_t new_sz = (size_t)(n + 1);
    seq_t *new_a = (seq_t *)malloc(new_sz * sizeof(seq_t));
    seq_t *new_s = (seq_t *)malloc(new_sz * sizeof(seq_t));
    if (!new_a || !new_s) return;           /* allocation failed – keep old */
    memcpy(new_a, a_cache, (size_t)(num_computed + 1) * sizeof(seq_t));
    memcpy(new_s, s_cache, (size_t)(num_computed + 1) * sizeof(seq_t));
    free(a_cache);
    free(s_cache);
    a_cache = new_a;
    s_cache = new_s;
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0 – return 0 */

    /* Expand caches if needed. */
    ensure_capacity(n);

    /* Compute any terms that are missing. */
    for (idx_t i = num_computed + 1; i <= n; ++i) {
        a_cache[i] = (s_cache[i - 1] + (seq_t)55) / 6;
        s_cache[i] = s_cache[i - 1] + a_cache[i];
    }
    num_computed = n;
    return a_cache[n];
}
