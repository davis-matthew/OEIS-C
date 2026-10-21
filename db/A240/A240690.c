/*
 * A240690  Number of partitions p of n such that p contains fewer 1s than its
 *          conjugate.
 * https://oeis.org/A240690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 135, u64 n <= 428,
 *            u128 n <= 1479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A240690: Number of partitions of n whose number of 1’s is
   smaller than that of its conjugate.
   Recurrence: a(1)=0,  a(n+1)=p(n)-a(n)   (n≥1)
   where p(k) is the ordinary partition number.  This uses the
   memorised partition numbers p(0..n) and the stored a(1..n).  The
   implementation recomputes the full partition table for each new
   n that exceeds the previously cached maximum; this keeps the
   algorithm simple and correct.  For n up to 10 000 the O(n²)
   complexity easily runs in well under two seconds. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *p_cache = NULL;   /* partition numbers up to max_cached_n */
static seq_t *a_cache = NULL;   /* a(n) values up to max_cached_n */
static idx_t  max_cached_n = 0;

/* Re‑allocate and recompute tables up to n (n≥1). */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached_n)
        return;                    /* already cached */

    /* Allocate fresh arrays. */
    seq_t *newp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *newa = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!newp || !newa) {
        free(newp);
        free(newa);
        exit(EXIT_FAILURE);
    }

    /* Compute partition numbers p[0..n] with the classic additive DP. */
    newp[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t m = k; m <= n; ++m) {
            newp[m] += newp[m - k];
        }
    }

    /* Compute sequence a using the recurrence. */
    if (n >= 1)
        newa[1] = (seq_t)0;          /* a(1)=0 */
    for (idx_t i = 2; i <= n; ++i) {
        newa[i] = newp[i - 1] - newa[i - 1];
    }

    /* Replace old tables. */
    free(p_cache);
    free(a_cache);
    p_cache = newp;
    a_cache = newa;
    max_cached_n = n;
}

/* Public entry point. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for n<1, harmless */
    ensure_cache(n);
    return a_cache[(size_t)n];
}
