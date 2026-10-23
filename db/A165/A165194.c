/*
 * A165194  Triangle of 2^(n-1) terms by rows, left half of (n+1)-th row = row
 *          n; right half = "reverse and increment" row n; using terms in
 *          A000110.
 * https://oeis.org/A165194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = Bell[bell_idx(n)], where bell_idx(n) is obtained by
 *            recursively mapping the position within its row. Bell[0] = 1;
 *            for m > 0, Bell[m] = Σ_{k=0}^{m-1} binom(m-1,k) * Bell[k]
 * Range:     u8 n <= 212, u16 n <= 1705, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   binom_n_k, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom_n_k.h"
#include "../utils/seq_one.h"

static seq_t *bell_cache = NULL;
static idx_t bell_cache_size = 0;

/* ensure bell_cache contains Bell[0..upto] */
static void _ensure_bell(idx_t upto)
{
    if (upto < bell_cache_size) return;

    /* allocate/grow storage */
    size_t new_cap = bell_cache_size > 0 ? bell_cache_size : 1;
    while (new_cap <= upto) new_cap <<= 1;

    if (bell_cache == NULL) {
        bell_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!bell_cache) { bell_cache_size = 0; return; }
        bell_cache[0] = seq_one();
        bell_cache_size = 1;
    } else {
        bell_cache = (seq_t *)realloc(bell_cache, new_cap * sizeof(seq_t));
        if (!bell_cache) { bell_cache_size = 0; return; }
    }

    for (idx_t m = bell_cache_size; m <= upto; ++m) {
        seq_t sum = 0;
        for (idx_t k = 0; k < m; ++k) {
            sum += binom_n_k(m - 1, k) * bell_cache[k];
        }
        bell_cache[m] = sum;
    }
    bell_cache_size = upto + 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* determine row r and position pos (1‑based) */
    idx_t r = (idx_t)(63 - __builtin_clzll(n)) + 1;          /* floor(log2(n))+1 */
    idx_t pos = n - ((idx_t)1 << (r - 1)) + 1;              /* within row */

    /* compute Bell index for this element */
    idx_t bell_idx = 0;
    while (r > 1) {
        idx_t half = ((idx_t)1) << (r - 2);  /* left half length */
        if (pos > half) {
            pos = half - (pos - half) + 1;   /* map to mirrored position */
            bell_idx++;                      /* +1 to index as per rule */
        }
        r--;
    }

    _ensure_bell(bell_idx);
    return bell_cache[bell_idx];
}
