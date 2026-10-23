/*
 * A169228  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^28 = I.
 * https://oeis.org/A169228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; for 1≤n≤27 a(n)=23·22^(n‑1); for n≥28
 *            a(n)=21·(a(n‑1)+…+a(n‑27))−231·a(n‑28)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* array of already computed terms   */
static size_t cache_cap = 0;     /* allocated capacity                */
static size_t cache_len = 0;     /* number of terms stored (0…len-1) */

/* enlarge cache so that indices 0 … n are available */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n) return;          /* already sufficient */

    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_mem) { exit(EXIT_FAILURE); } /* allocation failure */

    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i) new_mem[i] = cache[i];
        free(cache);
    }
    cache = new_mem;
    cache_cap = new_cap;
}

/* main routine: compute a(n) */
seq_t a(idx_t n)
{
    /* ensure we have storage for n */
    ensure_capacity(n);

    /* compute any terms that are not yet known   */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i <= 27) {                /* a(1)…a(27) */
            if (i == 1) {
                cache[1] = (seq_t)23;
            } else {
                cache[i] = cache[i-1] * (seq_t)22;
            }
        } else { /* recurrence for i ≥ 28 */
            seq_t sum = (seq_t)0;
            for (size_t k = 1; k <= 27; ++k) sum += cache[i - k];
            cache[i] = sum * (seq_t)21 - cache[i - 28] * (seq_t)231;
        }
    }

    cache_len = (size_t)n + 1;   /* now cache contains term n   */
    return cache[n];
}
