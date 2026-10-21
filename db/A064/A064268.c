/*
 * A064268  a(n) = (a(n-1) * a(n-6) + 2 * a(n-3) * a(n-4)) / a(n-7). a(1) =
 *          ... = a(7) = 1. Somos-7 variation.
 * https://oeis.org/A064268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1)*a(n-6)+2*a(n-3)*a(n-4))/a(n-7) for n>7;
 *            a(1)=a(2)=a(3)=a(4)=a(5)=a(6)=a(7)=1
 * Range:     u8 n <= 13, u16 n <= 17, u32 n <= 23, u64 n <= 31, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Memoise previously computed values. */
    static seq_t *buf = NULL;
    static size_t capacity = 0;
    static size_t max_computed = 0;      /* largest n with a[n] computed */

    /* Initialise buffer on first call. */
    if (buf == NULL) {
        capacity = 128;
        buf = (seq_t *)calloc(capacity, sizeof(seq_t));
        if (!buf) return (seq_t)0;      /* allocation failed – should not happen */
    }

    /* Grow buffer if needed. */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity;
        while ((size_t)n + 1 > new_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* allocation failed */
        buf = tmp;
        capacity = new_cap;
    }

    /* Compute missing values up to n. */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 7) {                    /* initial values */
            buf[i] = (seq_t)1;
        } else {
#ifdef __cplusplus
            /* C++ use overloaded operators (seq_t may be an arbitrary‑precision type). */
            seq_t term1 = buf[i - 1] * buf[i - 6];
            seq_t term2 = 2 * buf[i - 3] * buf[i - 4];
            seq_t numer = term1 + term2;
            buf[i] = numer / buf[i - 7];
#else
            /* C use unsigned __int128 for intermediate products (uint64_t case). */
            unsigned __int128 term1 = (unsigned __int128)buf[i - 1] * buf[i - 6];
            unsigned __int128 term2 = 2 * (unsigned __int128)buf[i - 3] * buf[i - 4];
            unsigned __int128 numer = term1 + term2;
            buf[i] = (seq_t)(numer / buf[i - 7]);
#endif
        }
    }

    max_computed = (size_t)n;
    return buf[n];
}
