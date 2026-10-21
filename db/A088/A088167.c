/*
 * A088167  Number of earlier occurring divisors of n; a(1)=1.
 * https://oeis.org/A088167
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 671, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A088167: Number of earlier occurring divisors of n.
   a(1) = 1; for n > 1,  a(n) = #{ k : 1 ≤ k < n  and  a(k) divides n }.
   The routine keeps a memoised table and counts divisors modulo i using
   pre‑stored integer values of a(k). */
/* Note: this implementation works for all supported seq_t/idx_t types. */

#include <stdlib.h>

/* Cache: cache[i-1] = a(i) as seq_t.  ui_cache holds the same values as
   idx_t to speed up the divisibility test. */
static seq_t *cache      = NULL;
static idx_t *ui_cache   = NULL;
static idx_t cache_len   = 0;      /* capacity of the arrays           */
static idx_t computed_max = 0;     /* highest n for which a(n) is known */

/* Ensure capacity for at least 'needed' elements. */
static void ensure_cache(idx_t needed)
{
    if (needed <= cache_len) return;
    idx_t new_len = cache_len ? cache_len * 2 : needed;
    if (new_len < needed) new_len = needed;

    /* realloc may return NULL – ignore errors here; the caller will
       terminate if memory is exhausted which is acceptable for an
       algorithmic problem. */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
    cache = tmp;
    idx_t *tmp_ui = (idx_t *)realloc(ui_cache, (size_t)new_len * sizeof(idx_t));
    ui_cache = tmp_ui;
    cache_len = new_len;
}

/* Public sequence access function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* invalid index */

    /* Compute values up to n if needed. */
    if (n > computed_max)
    {
        ensure_cache(n);

        /* initialise a(1) on first use */
        if (computed_max == 0)
        {
            cache[0] = (seq_t)1;
            ui_cache[0] = (idx_t)1;
            computed_max = 1;
        }

        /* fill in a(i) for i = computed_max+1 .. n */
        for (idx_t i = computed_max + 1; i <= n; ++i)
        {
            idx_t count = 0;
            /* count how many previous a(k) divide i */
            for (idx_t k = 1; k < i; ++k)
            {
                idx_t d = ui_cache[k-1];
                if (d != 0 && (i % d) == 0)
                    ++count;
            }
            cache[i-1] = (seq_t)count;
            ui_cache[i-1] = count;
        }

        computed_max = n;
    }

    return cache[n-1];
}
