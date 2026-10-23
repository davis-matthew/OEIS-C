/*
 * A014668  a(1) = 1, a(n) = Sum_{k=1..n-1} Sum_{d|k} a(d).
 * https://oeis.org/A014668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 a(n+1) = Σ_{k=1}^{n} floor(n/k) * a(k) (for n ≥ 1) This
 *            is equivalent to a(n) = Σ_{k=1}^{n-1} Σ_{d|k} a(d) as given in
 *            OEIS sequence A014668
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized implementation that keeps a[1..max_n] and its prefix sums
   pref[0..max_n] in static arrays.  pref[0] is always 0.
   The function works for any n ≥ 1, even with calls in arbitrary order.   */

static seq_t *a_cache = NULL;     /* a[1..max_n]                    */
static seq_t *pref_cache = NULL;  /* pref[0..max_n]                 */
static idx_t  cap = 0;            /* current allocated capacity     */
static idx_t  max_n = 0;          /* largest n already computed     */

/* Ensure that the static arrays can index at least up to idx (1‑based).  */
static void ensure_capacity(idx_t idx)
{
    if (idx + 1 > cap) {                 /* need space for indices 0..idx */
        size_t new_cap = (idx + 1) * 2;   /* grow with a factor 2            */
        seq_t *tmp_a = (seq_t *)realloc(a_cache,
                                        new_cap * sizeof(seq_t));
        seq_t *tmp_p = (seq_t *)realloc(pref_cache,
                                        new_cap * sizeof(seq_t));
        if (!tmp_a || !tmp_p) abort();   /* out of memory                   */
        a_cache = tmp_a;
        pref_cache = tmp_p;
        /* zero the newly allocated part   */
        for (idx_t i = cap; i < new_cap; ++i) {
            a_cache[i] = 0;
            pref_cache[i] = 0;
        }
        cap = new_cap;
        pref_cache[0] = 0;               /* prefix sum at position 0 */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* offset is 1, but remain safe */

    /* Return cached value if already known. */
    if (n <= max_n) return a_cache[n];

    /* First time initialisation. */
    if (max_n == 0) {
        ensure_capacity(1);
        a_cache[1] = (seq_t)1;
        pref_cache[1] = (seq_t)1;
        max_n = 1;
    }

    /* Extend the tables up to n. */
    if (n > max_n) {
        ensure_capacity(n);
        for (idx_t k = max_n + 1; k <= n; ++k) {
            /* Compute a[k] using the recurrence
               a[k] = Σ_{i=1}^{k-1} floor((k-1)/i) * a[i]          */
            idx_t m = k - 1;                /* number of terms in the sum */
            seq_t total = 0;
            idx_t l = 1;
            while (l <= m) {
                idx_t q = m / l;            /* floor(m / l)       */
                idx_t r = m / q;            /* last i with same q */
                seq_t range_sum = pref_cache[r] - pref_cache[l - 1];
                seq_t contrib = (seq_t)q * range_sum;
                total += contrib;
                l = r + 1;
            }
            a_cache[k] = total;
            pref_cache[k] = pref_cache[k - 1] + total;
        }
        max_n = n;
    }

    return a_cache[n];
}
