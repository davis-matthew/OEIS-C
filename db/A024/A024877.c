/*
 * A024877  a(n) = s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n-k+1), where k = [ n/2
 *          ], s = (natural numbers >= 3), t = (Lucas numbers).
 * https://oeis.org/A024877
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A024877: a(n) = Σ_{i=1}^{⌊n/2⌋} (i+2)·L_{n-i+1},
   where L_m is the m‑th Lucas number (L_0 = 2, L_1 = 1). */
#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------
 *  Generic helper to grow a dynamic array of seq_t values.
 * ------------------------------------------------------------ */
static void
ensure_seq_cap(seq_t **p, size_t *cap, idx_t required)
{
    if (*cap < (size_t)required) {
        size_t new_cap = (*cap == 0 ? required : (*cap << 1));
        if (new_cap < (size_t)required) new_cap = required;
        seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        if (!tmp) abort();                /* allocation failed */
        *p   = tmp;
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------
 *  Sequence A024877
 * ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* offset 2 → first listed term */

    /* ---- 1.  Build Lucas numbers up to index n ---- */
    static seq_t *lucas      = NULL;   /* L_m stored at index m  */
    static size_t lucas_cap  = 0;
    static idx_t  lucas_len  = 0;      /* next free index = highest+1 */

    if (lucas_len <= n) {
        ensure_seq_cap(&lucas, &lucas_cap, n + 1);
        if (lucas_len == 0) {
            lucas[0] = (seq_t)2;        /* L_0 */
            lucas[1] = (seq_t)1;        /* L_1 */
            lucas_len = 2;
        }
        for (idx_t m = lucas_len; m <= n; ++m)
            lucas[m] = lucas[m - 1] + lucas[m - 2];
        lucas_len = n + 1;
    }

    /* ---- 2.  Cache a(n) results so far ---- */
    static seq_t *cache      = NULL;   /* a(i) stored at index i   */
    static size_t cache_cap  = 0;
    static idx_t  cache_len  = 0;      /* next free index */

    if (cache_len <= n) {
        ensure_seq_cap(&cache, &cache_cap, n + 1);
        for (idx_t i = cache_len; i <= n; ++i) {
            idx_t k   = i >> 1;              /* floor(i/2) */
            seq_t sum = 0;
            for (idx_t j = 1; j <= k; ++j) {
                sum += ((seq_t)(j + 2)) * lucas[i - j + 1];
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
