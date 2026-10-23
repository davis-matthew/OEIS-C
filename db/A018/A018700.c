/*
 * A018700  Divisors of 882.
 * https://oeis.org/A018700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Divisors of 882 (OEIS A018700)
   a(n) = the n‑th divisor of 882 in ascending order,
   1 ≤ n ≤ 18.  For n outside this range a( n ) = 0. */

#include <stdlib.h>
#include <stddef.h>

/* Comparator for qsort that works for any seq_t supporting < and >. */
static int cmp_seq(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* Return the n-th divisor of 882 (sorted ascending). */
seq_t a(idx_t n)
{
    /* Prime powers for 882 = 2 * 3^2 * 7^2. */
    static const seq_t p2[2] = { (seq_t)1, (seq_t)2 };
    static const seq_t p3[3] = { (seq_t)1, (seq_t)3, (seq_t)9 };
    static const seq_t p7[3] = { (seq_t)1, (seq_t)7, (seq_t)49 };

    /* Storage for the 18 divisors, computed lazily. */
    static seq_t divisors[18];
    static size_t cnt = 0;

    if (cnt == 0) {
        size_t idx = 0;
        for (size_t i = 0; i < 2; ++i)
            for (size_t j = 0; j < 3; ++j)
                for (size_t k = 0; k < 3; ++k)
                    divisors[idx++] = p2[i] * p3[j] * p7[k];
        cnt = 18;
        qsort(divisors, cnt, sizeof(seq_t), cmp_seq);
    }

    if (n == 0 || n > cnt)
        return (seq_t)0;        /* undefined index */
    return divisors[n - 1];
}
