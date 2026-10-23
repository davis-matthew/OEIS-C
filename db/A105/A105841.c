/*
 * A105841  Numbers n such that n^2 = 31*m^2 + 31*m + 1.
 * https://oeis.org/A105841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=309, a(3)=50469, a(4)=8242961, a(5)=17484559,
 *            a(6)=2855704251, a(7)=466414209771, a(8)=76178131891679. For n >
 *            8: a(n) = 9241598 * a(n-4) - a(n-8) (repeated recurrence
 *            generates all subsequent terms) This implementation memoises the
 *            sequence in a static array, allocating enough space...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static const seq_t init[8] = {
        (seq_t)1ULL,
        (seq_t)309ULL,
        (seq_t)50469ULL,
        (seq_t)8242961ULL,
        (seq_t)17484559ULL,
        (seq_t)2855704251ULL,
        (seq_t)466414209771ULL,
        (seq_t)76178131891679ULL
    };
    const seq_t C = (seq_t)9241598ULL;

    static seq_t *buf = NULL;   /* array of computed terms, 0‑based */
    static idx_t capacity = 0;  /* allocated size */
    static idx_t stored  = 0;   /* number of terms currently stored */

    /* initialise storage on first call */
    if (buf == NULL) {
        capacity = 64;                     /* arbitrary initial capacity */
        buf = (seq_t *)malloc(sizeof(seq_t) * capacity);
        for (idx_t i = 0; i < 8 && i < capacity; ++i)
            buf[i] = init[i];
        stored = 8;
    }

    /* if requested n is already stored, return it immediately */
    if (n <= stored)
        return buf[n - 1];

    /* extend capacity if needed */
    if (n > capacity) {
        capacity = n;                     /* grow exactly to required size */
        buf = (seq_t *)realloc(buf, sizeof(seq_t) * capacity);
    }

    /* compute new terms up to n (inclusive) */
    for (idx_t i = stored; i < n; ++i) {
        /* for i < 8 the recurrence would refer to indices < 0,
           but we already have the first 8 terms.  For i >= 8
           the formula a(i+1) = C * a(i-3) - a(i-7) holds. */
        seq_t val = C * buf[i - 4] - buf[i - 8];
        buf[i] = val;
    }

    stored = n;                          /* update number of stored terms */
    return buf[n - 1];
}
