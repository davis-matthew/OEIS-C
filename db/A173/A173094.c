/*
 * A173094  a(1)=2, a(2)=3, a(n)=a(n-2)*a(n-1)-a(n-1).
 * https://oeis.org/A173094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=3, a(n)=a(n‑1)*(a(n‑2)-1) for n≥3
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for previously computed values; index 0 is unused. */
static seq_t *cache = NULL;
static size_t cap   = 0;     /* number of slots allocated (index 0 unused) */

/* Ensures that cache[1..n] contains the correct sequence values. */
static void
ensure_cache(idx_t n)
{
    if (n < cap) return;            /* already computed */

    size_t new_cap = (size_t)(n + 1);          /* slots needed (index n included) */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();               /* allocation failed */
    cache = tmp;

    idx_t start = (idx_t)(cap == 0 ? 1 : cap);
    for (idx_t i = start; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)2;
        } else if (i == 2) {
            cache[2] = (seq_t)3;
        } else {
            cache[i] = cache[i - 1] * (cache[i - 2] - (seq_t)1);
        }
    }
    cap = new_cap;
}

/* Main sequence function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n<1, return 0 */
    ensure_cache(n);
    return cache[n];
}
