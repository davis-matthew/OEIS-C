/*
 * A061191  Numbers of the form (10*a + b)^2 + (10*b + a)^2 with a and b less
 *          than 10, in numerical order.
 * https://oeis.org/A061191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, fini, full, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Enumerate all pairs (a,b) with 0 ≤ a,b < 10,
   compute (10a+b)^2 + (10b+a)^2, keep one value per unordered pair,
   sort the results, and return the n‑th smallest (1‑based). */

#include <stdlib.h>

/* Comparator for qsort – works for numeric types and for a
   class type that overloads operator< (C++). */
static int cmp_seq(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for indices ≤ 0 */

    /* There are exactly 55 distinct values (unordered pairs a,b). */
    seq_t vals[55];
    idx_t cnt = 0;

    for (idx_t a = 0; a < 10; ++a) {
        for (idx_t b = 0; b < 10; ++b) {
            if (a <= b) { /* avoid duplicates from swapping a and b */
                seq_t t1 = (seq_t)(10 * a + b);
                seq_t t2 = (seq_t)(10 * b + a);
                vals[cnt++] = t1 * t1 + t2 * t2;
            }
        }
    }

    /* Sort the 55 values */
    qsort(vals, cnt, sizeof(seq_t), cmp_seq);

    /* Return the requested term if it exists */
    if (n - 1 >= cnt) return 0;   /* out of range – undefined */
    return vals[n - 1];
}
