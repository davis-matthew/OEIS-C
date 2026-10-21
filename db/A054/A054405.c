/*
 * A054405  Row sums of array T as in A055215.
 * https://oeis.org/A054405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n‑1) + a(n‑2) – a(n‑4); a(0)=1, a(1)=2, a(2)=3, a(3)=5
 * Range:     u8 n <= 15, u16 n <= 35, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_capacity(seq_t **arr, idx_t *cap, idx_t need)
{
    if (*cap >= need) return;
    idx_t new_cap = (*cap == 0) ? 16 : *cap;
    if (new_cap < need) new_cap = need;
    while (new_cap < need) new_cap <<= 1;
    *arr = (seq_t *)realloc(*arr, (size_t)new_cap * sizeof(seq_t));
    *cap = new_cap;
}

static seq_t *memo   = NULL;   /* memoized sequence values             */
static idx_t  memsz  = 0;       /* highest stored index + 1             */
static idx_t  memcap = 0;       /* allocated capacity of memo array      */

seq_t a(idx_t n)
{
    /* Direct returns for the first four terms.  No memoization required. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;

    /* n >= 4 : ensure base terms are stored. */
    if (memsz <= 3) {
        /* Allocate at least four slots. */
        idx_t needed_cap = 4;
        if (needed_cap > memcap) {
            memo = (seq_t *)realloc(memo, (size_t)needed_cap * sizeof(seq_t));
            memcap = needed_cap;
        }
        memo[0] = (seq_t)1;
        memo[1] = (seq_t)2;
        memo[2] = (seq_t)3;
        memo[3] = (seq_t)5;
        memsz = 4;
    }

    /* Extend memoization up to index n if necessary. */
    if (n >= memsz) {
        ensure_capacity(&memo, &memcap, n + 1);
        for (idx_t i = memsz; i <= n; ++i) {
            memo[i] = memo[i-1] + memo[i-2] - memo[i-4];
        }
        memsz = n + 1;
    }

    return memo[n];
}
