/*
 * A097562  a(n) = number of terms among {a(1), a(2), a(3), ..., a(n-1)} that
 *          are coprime to n; a(1)=5.
 * https://oeis.org/A097562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of terms among a(1), a(2), …, a(n‑1) that are
 *            coprime to n; a(1)=5. No closed formula; computed by incremental
 *            construction using gcd
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoized: previously computed terms are stored in
   a static array and recomputed only for new indices. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"   /* provides seq_t gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Static cache of computed sequence values.
       The array is 1‑based; seq[0] is unused. */
    static seq_t *seq = NULL;
    static size_t alloc = 0;      /* allocated size */
    static idx_t max_computed = 0;/* largest n for which seq[n] is valid */

    /* Guard against invalid indices: return 0 for n==0, which is never
       expected by the sequence definition. */
    if (n == 0)
        return (seq_t)0;

    /* Ensure the cache array is large enough for index n. */
    if (seq == NULL) {
        alloc = 16;
        seq = (seq_t *)malloc(alloc * sizeof(seq_t));
        if (!seq) return (seq_t)0;    /* allocation failure fallback */
        seq[0] = 0;                  /* unused entry for 1‑based indexing */
    }
    if ((size_t)n >= alloc) {
        size_t new_alloc = alloc * 2;
        while ((size_t)n >= new_alloc)
            new_alloc *= 2;
        seq = (seq_t *)realloc(seq, new_alloc * sizeof(seq_t));
        if (!seq) return (seq_t)0;    /* allocation failure fallback */
        alloc = new_alloc;
    }

    /* Compute missing values up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i == 1) {
            seq[1] = (seq_t)5;
            continue;
        }

        seq_t cnt = 0;
        seq_t sn = (seq_t)i;          /* cast index to seq_t once */
        for (idx_t k = 1; k < i; ++k) {
            if (gcd(seq[k], sn) == (seq_t)1)
                ++cnt;
        }
        seq[i] = cnt;
    }

    max_computed = (max_computed > n) ? max_computed : n;
    return seq[n];
}
