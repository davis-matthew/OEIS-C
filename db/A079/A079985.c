/*
 * A079985  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={-1,0,1}.
 * https://oeis.org/A079985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A079985: Number of permutations satisfying –k ≤ p(i)–i ≤ r and
 * p(i)–i ∉ I, with k = 3, r = 3, I ={ –1, 0, 1 }
 * a(n) = a(n–1)+a(n–2)–a(n–3)+3a(n–4)+2a(n–5)+2a(n–6)
 *        –6a(n–7)–2a(n–8)+2a(n–9)–3a(n–10)–a(n–11)–a(n–12)
 *        +a(n–13)+a(n–14)
 * a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 * a(4)=1, a(5)=2, a(6)=4, a(7)=0, a(8)=4, a(9)=10, a(10)=29,
 * a(11)=40, a(12)=64, a(13)=88
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static const seq_t init_vals[14] = {
    1, 0, 0, 0, 1, 2, 4, 0, 4, 10, 29, 40, 64, 88
};

static seq_t *cache = NULL;      /* cached terms */
static size_t cache_len = 0;     /* number of valid terms in cache */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure that cache contains terms up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;

    /* Initialise cache to hold at least 14 initial terms. */
    if (cache_cap < 14) {
        seq_t *new_ptr = (seq_t *)realloc(cache, 14 * sizeof(seq_t));
        if (!new_ptr) {
            abort();
        }
        cache = new_ptr;
        cache_cap = 14;
        for (size_t i = 0; i < 14; ++i) {
            cache[i] = init_vals[i];
        }
        cache_len = 14;
    }

    /* Grow cache if needed. */
    if (need > cache_cap) {
        seq_t *new_ptr = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!new_ptr) {
            abort();
        }
        cache = new_ptr;
        cache_cap = need;
    }

    /* Compute remaining terms. */
    for (size_t i = cache_len; i < need; ++i) {
        /* Recurrence uses terms i‑1 … i‑14 */
        seq_t val =
            cache[i - 1] + cache[i - 2] - cache[i - 3] + (seq_t)3 * cache[i - 4]
            + (seq_t)2 * cache[i - 5] + (seq_t)2 * cache[i - 6]
            - (seq_t)6 * cache[i - 7] - (seq_t)2 * cache[i - 8]
            + (seq_t)2 * cache[i - 9] - (seq_t)3 * cache[i -10]
            - cache[i -11] - cache[i -12] + cache[i -13] + cache[i -14];
        cache[i] = val;
    }

    cache_len = need;
}

/* Public function. */
seq_t a(idx_t n)
{
    if (n < 14) {
        return init_vals[(size_t)n];
    }
    ensure_cache(n);
    return cache[(size_t)n];
}
