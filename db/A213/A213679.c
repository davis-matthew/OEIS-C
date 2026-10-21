/*
 * A213679  Total sum of parts <= n of multiplicity 0 in all partitions of n.
 * https://oeis.org/A213679
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1}^n i*(p(n)-p(n-i)) equivalently a(n) =
 *            (n*(n+1)/2)*p(n) - ( n*sum_{k=0}^{n-1}p(k) -
 *            sum_{k=0}^{n-1}k*p(k) ) where p(k) is A000041(k), the partition
 *            number
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 65, u64 n <= 261,
 *            u128 n <= 1000, big any n
 * Helpers:   free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <string.h>   /* memcpy */
#include "../utils/free_seq_array.h"

/* Helper to allocate arrays for arbitrary‑precision seq_t types. */
static seq_t *alloc_seq_array(size_t n)
{
    /* In the provided utilities this is defined.  It simply
       allocates \`n\` elements of type seq_t with appropriate
       constructors/destructors. */
    return (seq_t *)malloc(n * sizeof(seq_t));
}

static seq_t *p   = NULL;       /* p[i] = partition number of i */
static seq_t *s1  = NULL;       /* s1[i] = sum_{k=0}^{i-1} p[k] */
static seq_t *s2  = NULL;       /* s2[i] = sum_{k=0}^{i-1} k*p[k] */
static size_t  cap   = 0;       /* current allocated capacity (>= needed) */
static idx_t   high  = (idx_t)-1;/* highest index for which arrays are filled */

/* Ensure arrays are allocated up to n (inclusive) and values are computed. */
static void ensure_up_to(idx_t n)
{
    if (high != (idx_t)-1 && n <= high) return;

    /* Grow capacity if needed */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;

        seq_t *newp  = alloc_seq_array(new_cap);
        seq_t *new_s1 = alloc_seq_array(new_cap);
        seq_t *new_s2 = alloc_seq_array(new_cap);

        if (p)   memcpy(newp,   p,   cap * sizeof(seq_t));
        if (s1)  memcpy(new_s1, s1,  cap * sizeof(seq_t));
        if (s2)  memcpy(new_s2, s2,  cap * sizeof(seq_t));

        if (p)   free_seq_array(p);
        if (s1)  free_seq_array(s1);
        if (s2)  free_seq_array(s2);

        p   = newp;
        s1  = new_s1;
        s2  = new_s2;
        cap = new_cap;
    }

    /* Initialise freshly allocated entries (if any) */
    if (high == (idx_t)-1) {
        for (size_t i = 0; i < cap; ++i) {
            p[i]  = (seq_t)0;
            s1[i] = (seq_t)0;
            s2[i] = (seq_t)0;
        }
        p[0] = (seq_t)1;   /* p(0) = 1  */
        high = 0;
    }

    /* Compute values from high+1 up to n */
    for (idx_t i = high + 1; i <= n; ++i) {
        seq_t pos = (seq_t)0, neg = (seq_t)0;
        int   sign = 1;  /* sign for the current generalized pentagonal number */

        for (int k = 1; ; ++k) {
            idx_t pent1 = (idx_t)k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            if (sign > 0) pos += p[i - pent1];
            else          neg += p[i - pent1];

            idx_t pent2 = (idx_t)k * (3 * k + 1) / 2;
            if (pent2 > i) { sign = -sign; continue; }
            if (sign > 0) pos += p[i - pent2];
            else          neg += p[i - pent2];
            sign = -sign;
        }
        p[i] = pos - neg;  /* partitions of i */

        s1[i] = s1[i - 1] + p[i - 1];
        s2[i] = s2[i - 1] + ((seq_t)(i - 1)) * p[i - 1];
    }
    high = n;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);

    seq_t term1 = (seq_t)n * (n + 1) / 2;
    seq_t part  = p[n];
    seq_t sub   = ((seq_t)n) * s1[n] - s2[n];

    return term1 * part - sub;
}
