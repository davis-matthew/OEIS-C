/*
 * A026110  a(n) = number of (s(0), s(1), ..., s(n)) such that every s(i) is a
 *          nonnegative integer, s(0) = 0, s(1) = 1, s(n) = 4, |s(i) - s(i-1)|
 *          <= 1 for i >= 2. Also a(n) = T(n,n-4), where T is the array
 *          defined in A026105.
 * https://oeis.org/A026110
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 18, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A026110:  a(n) = number of walks (s(0),…,s(n)) with
 * s(0)=0, s(1)=1, s(n)=4 and |s(i)-s(i-1)| ≤ 1 for i≥2.
 *
 * Recurrence (valid for n ≥ 8):
 *      a(n) = [ (4n²‑n‑51)a(n‑1) + (‑2n²+11n+18)a(n‑2)
 *               - (4n‑1)(n‑3)a(n‑3) + 3(n‑3)(n‑4)a(n‑4) ]
 *            / [(n+6)(n‑4)]
 *
 * Initial values:
 *      a(4)=1, a(5)=4, a(6)=15, a(7)=50
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* cache of computed terms */
static idx_t  cache_size    = 0;     /* allocated size of cache   */
static idx_t  computed_up_to = 0;    /* first index yet uncomputed */

/* Ensure the cache can address indices up to n (inclusive). */
static void ensure_cache_size(idx_t n)
{
    if ((idx_t)(n + 1) > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();   /* allocation failure */
        cache = new_cache;
        for (size_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }
}

/* Compute a(k) for all k with   computed_up_to ≤ k ≤ n   */
static void compute_up_to(idx_t n)
{
    for (idx_t k = computed_up_to; k <= n; ++k) {
        /* Base cases */
        if (k < 8) {
            switch (k) {
                case 4:  cache[k] = 1;  break;
                case 5:  cache[k] = 4;  break;
                case 6:  cache[k] = 15; break;
                case 7:  cache[k] = 50; break;
            }
            continue;
        }

        /* Recurrence for n ≥ 8 */
        int64_t nn = (int64_t)k;                 /* signed 64–bit safe */
        int64_t coeff1 = 4*nn*nn - nn - 51;     /* positive for n≥4  */
        int64_t coeff2 = -2*nn*nn + 11*nn + 18; /* may be negative   */
        int64_t coeff3 = (4*nn - 1)*(nn - 3);    /* positive          */
        int64_t coeff4 = 3*(nn - 3)*(nn - 4);    /* positive          */

        seq_t term1 = (seq_t)coeff1 * cache[k-1];

        seq_t term2;                               /* |coeff2| * a(k-2) */
        if (coeff2 >= 0)
            term2 = (seq_t)coeff2 * cache[k-2];
        else
            term2 = (seq_t)(-coeff2) * cache[k-2];

        seq_t term3 = (seq_t)coeff3 * cache[k-3];
        seq_t term4 = (seq_t)coeff4 * cache[k-4];

        seq_t numerator = term1;
        if (coeff2 >= 0)
            numerator += term2;
        else
            numerator -= term2;
        numerator -= term3;
        numerator += term4;

        seq_t denominator = ((seq_t)k + 6) * ((seq_t)k - 4);
        cache[k] = numerator / denominator;
    }
    computed_up_to = n + 1;
}

/* Public accessor */
seq_t a(idx_t n)
{
    if (n < 4)                     /* undefined before offset  */
        return 0;

    ensure_cache_size(n);
    if (computed_up_to <= n)
        compute_up_to(n);
    return cache[n];
}
