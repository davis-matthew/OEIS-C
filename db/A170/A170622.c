/*
 * A170622  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1. For 1 ≤ n ≤ 47: a(n) = 2 + 35 × Σ_{k=1}^{n} a(n‑k). For
 *            n = 48: a(48) = 1 + 35 × Σ_{k=1}^{47} a(48‑k). For n ≥ 49: a(n)
 *            = 35 × Σ_{k=1}^{47} a(n‑k) – 630 × a(n‑48). This recurrence
 *            follows from the rational generating function G(t) =
 *            (t^48+2t^47+…+2t+1)/(630t^48–35t^47–…–35t+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* External types supplied by the build system. */
typedef idx_t  idx_t_for_seq;  /* placeholder: actual type is provided externally */

static seq_t *cache = NULL;
static size_t cache_size = 0;

static void
ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;

    size_t new_size = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Out of memory – terminate program safely. */
        abort();
    }
    cache = tmp;

    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        idx_t max_k = (i <= 47) ? i : 47;   /* for 0 ≤ i ≤ 48   */
        /* if i > 48, max_k should be 47 */
        if (i > 48) max_k = 47;

        for (idx_t k = 1; k <= max_k; ++k)
            sum += cache[(size_t)(i - k)];

        seq_t N;
        if (i == 0 || i == 48)
            N = 1;
        else if (i <= 47)
            N = 2;
        else
            N = 0;

        if (i <= 48) {
            /* a(i) = N + 35 * sum   (valid for i = 0 … 48) */
            cache[(size_t)i] = N + 35 * sum;
        } else {
            /* a(i) = 35 * sum – 630 * a(i-48)   (for i ≥ 49) */
            cache[(size_t)i] = 35 * sum - cache[(size_t)(i - 48)] * 630;
        }
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
