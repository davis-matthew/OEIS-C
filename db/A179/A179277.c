/*
 * A179277  A(x) = C(x) * C(x^2) * C(x^4) * C(x^8) *...; C = Catalan, A000108.
 * https://oeis.org/A179277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A179277: a(n) = Sum_{l=0..⌊n/2⌋} a(l) * Catalan(n‑2l)
   (Catalan numbers A000108).  The recurrence starts with
   a(0)=1, a(1)=1.  This implementation memoises a(n) and the
   Catalan numbers needed for the recurrence.  The algorithm is
   O(n²) time and O(n) memory, easily fast enough for n≈10⁴. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/catalan.h"   /* provides catalan(idx_t) */

static seq_t *cat_cache = NULL;   /* cached Catalan numbers          */
static size_t   cat_cap   = 0;     /* allocated size for cat_cache    */
static idx_t    cat_len   = 0;     /* how many Catalan values are set */

static seq_t *a_cache  = NULL;    /* cached a(n) values              */
static size_t   a_cap   = 0;       /* allocated size for a_cache      */
static idx_t    a_len   = 0;       /* last index that has been filled */

/* Ensure cat_cache has capacity for indices 0..n and that Catalan(n)
   through Catalan(n) are stored.  cat_len tracks how far we have
   already computed. */
static void ensure_cat(idx_t n)
{
    if (cat_len > n) return;                    /* already have up to n */

    /* requested length */
    idx_t need = n + 1;
    /* Grow capacity if necessary */
    if (cat_cap < need) {
        idx_t new_cap = cat_cap ? cat_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        cat_cache = (seq_t *)realloc(cat_cache, new_cap * sizeof(seq_t));
        cat_cap = new_cap;
    }

    /* Fill the new entries */
    for (idx_t i = cat_len; i <= n; ++i)
        cat_cache[i] = catalan(i);

    cat_len = n + 1;
}

/* Ensure a_cache has capacity for indices 0..n and that a(k)
   for k in [a_len .. n] are computed.  a_len tracks the largest
   index that has been computed. */
static void ensure_a(idx_t n)
{
    if (a_len > n) return;                    /* already computed up to n */

    /* requested length */
    idx_t need = n + 1;
    /* Grow capacity if needed */
    if (a_cap < need) {
        idx_t new_cap = a_cap ? a_cap * 2 : 16;
        while (new_cap < need) new_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }

    /* Compute new terms */
    for (idx_t m = a_len; m <= n; ++m) {
        if (m == 0 || m == 1) {
            a_cache[m] = 1;                     /* a(0)=a(1)=1 */
        } else {
            seq_t sum = 0;
            for (idx_t l = 0; l <= m / 2; ++l) {
                idx_t k = m - 2 * l;
                ensure_cat(k);
                sum += a_cache[l] * cat_cache[k];
            }
            a_cache[m] = sum;
        }
    }
    a_len = n + 1;
}

/* Public entry point – computes a(n). */
seq_t a(idx_t n)
{
    ensure_a(n);
    return a_cache[n];
}
