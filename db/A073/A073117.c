/*
 * A073117  a(n+1) = a(n) + a(n) mod n; a(1) = 1.
 * https://oeis.org/A073117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 674, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 1
 *               a(n+1) = a(n) + (a(n) mod n)   for n ≥ 1
 * (The “mod” operand is the previous index n, so a(2) = a(1)+(a(1) mod 1), etc.) */

#include <stdlib.h>

static seq_t *a_cache = NULL;   /* cache[0] holds a(1), cache[1] holds a(2), ... */
static size_t a_cache_len = 0;  /* number of terms currently stored */
static size_t a_cache_cap = 0;  /* allocated capacity */

/* Ensure the cache can hold at least `need` terms. */
static void ensure_a_cache(size_t need) {
    if (need <= a_cache_cap)
        return;
    size_t new_cap = (a_cache_cap ? a_cache_cap * 2 : 16);
    if (new_cap < need)
        new_cap = need;
    seq_t *new_buf = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    if (!new_buf)
        abort();                /* allocation failure */
    a_cache = new_buf;
    a_cache_cap = new_cap;
}

/* Compute terms up to `n` inclusive (1‑based index). */
static void compute_a_up_to(idx_t n) {
    if (n == 0)
        return;                /* no definition for n = 0 */
    if (a_cache_len >= (size_t)n)
        return;                /* already computed */

    ensure_a_cache((size_t)n);

    /* First term */
    if (a_cache_len == 0) {
        a_cache[0] = (seq_t)1;   /* a(1) = 1 */
        a_cache_len = 1;
    }

    /* Append terms a(len+1) for len = a_cache_len .. n-1 */
    for (size_t len = a_cache_len; len < (size_t)n; ++len) {
        /* len is the current index of a(len) (1‑based) */
        seq_t prev = a_cache[len-1];    /* a(len) */
        seq_t rem = prev % (seq_t)len;  /* a(len) mod len */
        a_cache[len] = prev + rem;      /* a(len+1) */
    }

    a_cache_len = (size_t)n;
}

seq_t a(idx_t n) {
    if (n == 0)   /* undefined, but guard to avoid UB */
        return (seq_t)0;
    compute_a_up_to(n);
    return a_cache[(size_t)(n-1)];
}
