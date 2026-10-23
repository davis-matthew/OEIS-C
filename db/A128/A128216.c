/*
 * A128216  a(1)=a(2)=1. a(n+1) = a(n) + a(smallest prime dividing n).
 * https://oeis.org/A128216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=1; a(n+1)=a(n)+a(p), where p is the smallest prime
 *            dividing n
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 131, u64 n <= 311,
 *            u128 n <= 719, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ---------- helper: smallest prime factor sieve ---------- */
static idx_t *spf_cache = NULL;   /* indices start at 0 */
static idx_t spf_cap   = 0;

/* Build/extend the smallest‑prime‑factor table up to 'limit'. */
static void init_spf(idx_t limit)
{
    if (limit < 2) return;
    if (limit <= spf_cap) return;   /* already built */

    /* reallocate */
    idx_t *new_cache = (idx_t *)realloc(spf_cache,
                                        (size_t)(limit + 1) * sizeof(idx_t));
    if (!new_cache) {
        /* allocation failure – fallback to minimal table */
        return;
    }
    spf_cache = new_cache;

    /* initialise */
    for (idx_t i = 0; i <= limit; ++i)
        spf_cache[i] = i;

    /* linear sieve for SPF: O(limit log log limit) */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf_cache[i] == i) {           /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf_cache[j] == j)
                    spf_cache[j] = i;
            }
        }
    }
    spf_cap = limit;
}

/* ---------- sequence cache ---------- */
static seq_t *seq_cache   = NULL;
static idx_t  seq_cap     = 0;  /* memory capacity in elements, may be > cached */
static idx_t  seq_computed = 0; /* max index already computed */

/* Ensure that the cache can hold at least 'n' indices (1‑based). */
static void ensure_seq_cache(idx_t n)
{
    if (n + 1 <= seq_cap) return;                     /* capacity already enough */

    idx_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(seq_cache,
                                        (size_t)new_cap * sizeof(seq_t));
    if (!new_cache) return;                           /* allocation fail */
    seq_cache = new_cache;
    seq_cap = new_cap;
    /* initialise new entries to zero */
    for (idx_t i = seq_cap; i < new_cap; ++i)
        seq_cache[i] = 0;
}

/* ---------- main function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined for 0 – return 0 safely */

    /* make sure we have SPF information up to n-1 */
    if (n > 1)
        init_spf(n - 1);

    /* extend the sequence cache if needed */
    ensure_seq_cache(n);

    /* compute missing terms up to n */
    for (idx_t k = seq_computed + 1; k <= n; ++k) {
        if (k == 1 || k == 2) {
            seq_cache[k] = 1;
        } else {
            idx_t spf = spf_cache[k - 1];      /* smallest prime dividing k-1 */
            seq_cache[k] = seq_cache[k - 1] + seq_cache[ (idx_t)spf ];
        }
    }
    seq_computed = n;

    return seq_cache[n];
}
