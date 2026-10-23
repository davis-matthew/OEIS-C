/*
 * A036657  Number of n-node rooted unlabeled trees with out-degree <=2 and
 *          exactly 2 edges at the root.
 * https://oeis.org/A036657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^{⌊(n‑1)/2⌋} t_i * t_{n‑1‑i} * + (if (n‑1) even)
 *            t_{(n‑1)/2}*(t_{(n‑1)/2}+1)/2 * where t_1=1, t_n = t_{n‑1} +
 *            a(n). * a(n) counts rooted unlabeled trees with out‑degree ≤2
 *            and * exactly two children of the root
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 30, u64 n <= 56, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *t_cache   = NULL;   /* array holding t_n values */
static size_t t_cap     = 0;      /* allocated capacity (in number of entries) */
static size_t t_len     = 0;      /* number of entries that are already valid */

static void ensure_computed(idx_t n)
{
    if (t_len > 0 && t_len-1 >= n) return;          /* already computed */

    /* --- Make sure we have enough storage --- */
    if (t_cap == 0) {
        t_cap = (size_t)n + 1;
        t_cache = (seq_t *)calloc(t_cap, sizeof(seq_t));
        if (!t_cache) abort();
    } else if (t_cap <= (size_t)n) {
        size_t new_cap = t_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(t_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        t_cache = tmp;
        t_cap = new_cap;
    }

    /* --- Initialise base values if starting from scratch --- */
    if (t_len == 0) {
        t_cache[0] = (seq_t)0;
        if (t_cap > 1) t_cache[1] = (seq_t)1;
        t_len = (t_cap > 1) ? 2 : 1;
    }

    /* --- Compute entries up to n --- */
    for (idx_t i = (idx_t)t_len; i <= n; ++i) {
        idx_t k = i - 1;                    /* total nodes in the two subtrees */
        seq_t sum = 0;
        idx_t half = k / 2;

        for (idx_t j = 1; j <= half; ++j) {
            idx_t other = k - j;
            if (j < other) {
                sum += t_cache[j] * t_cache[other];
            } else { /* j == other */
                sum += t_cache[j] * (t_cache[j] + (seq_t)1) / (seq_t)2;
            }
        }
        t_cache[i] = t_cache[i - 1] + sum;
    }
    t_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)0;     /* a(0) = a(1) = a(2) = 0 */

    ensure_computed(n);
    return t_cache[n] - t_cache[n - 1];
}
