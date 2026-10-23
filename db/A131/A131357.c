/*
 * A131357  a(1) = 1, a(2) = 2; for n > 1, a(n) = sum of the next two smallest
 *          integers > a(n-1) which are coprime to the sum s = a(1) + ... +
 *          a(n-1).
 * https://oeis.org/A131357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>1, a(n)=n1+n2 where n1,n2 are the next two
 *            smallest integers > a(n-1) coprime to s = sum_{i=1}^{n-1} a(i)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /* static cache */
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static size_t size = 0;      /* number of terms currently stored */
    static seq_t sum = 0;        /* cumulative sum of stored terms */

    if (n == 0) return 0;         /* should not happen, safe fallback */

    /* ensure capacity for required number of terms */
    if (cap < (size_t)n) {
        size_t newcap = cap ? cap : 16;
        while (newcap < (size_t)n) newcap <<= 1;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* compute missing terms */
    for (idx_t idx = (idx_t)(size + 1); idx <= n; ++idx) {
        if (idx == 1) {
            seq[0] = (seq_t)1;
            sum = (seq_t)1;
        } else if (idx == 2) {
            seq[1] = (seq_t)2;
            sum += (seq_t)2;
        } else {
            seq_t prev = seq[idx - 2];
            seq_t m1 = prev + (seq_t)1;
            while (gcd(m1, sum) != (seq_t)1) m1++;

            seq_t m2 = m1 + (seq_t)1;
            while (gcd(m2, sum) != (seq_t)1) m2++;

            seq_t next = m1 + m2;
            seq[idx - 1] = next;
            sum += next;
        }
    }

    size = (size_t)n;
    return seq[n - 1];
}
