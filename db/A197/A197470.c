/*
 * A197470  Number of nX3 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 0,1,0,0,0 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A197470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the recurrence a(n) = 6*a(n-2) + 3*a(n-3) +
 *            4*a(n-4) - 2*a(n-5) + 2*a(n-6) - 7*a(n-7) - 19*a(n-8) -
 *            23*a(n-9) - 19*a(n-10) - 19*a(n-11) - 4*a(n-12) - 4*a(n-13) (n >
 *            16) with initial values a(1)=3, a(2)=6, a(3)=26, a(4)=46,
 *            a(5)=181, a(6)=374, a(7)=1300, a(8)=2886, a(9)=9446, a(10)=...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper: multiply a seq_t by a small constant without overflow.
   For small seq_t types (≤8 bytes) we use unsigned __int128 to hold
   the product exactly; otherwise rely on the seq_t implementation. */
static inline seq_t mul_const(seq_t v, unsigned int c)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 tmp = (unsigned __int128)v * c;
        return (seq_t)tmp;
    } else {
        return v * (seq_t)c;
    }
}

seq_t a(idx_t n)
{
    /* Initial values up to a(16) */
    static const seq_t init[17] = {
        0,          /* dummy 0 index */
        3, 6, 26, 46, 181, 374, 1300, 2886, 9446, 22060,
        69096, 167466, 507600, 1265434, 3741192, 9529514
    };

    /* Static storage for computed values and memoisation. */
    static seq_t *buf = NULL;      /* array of a(1)...a(K) */
    static idx_t  cap = 0;         /* current allocated capacity (>= last stored index) */
    static idx_t  last = 0;        /* largest n for which a(n) is already stored */

    /* Allocate initial storage if first call. */
    if (!buf) {
        cap = 16;                      /* accommodate initial 16 values */
        buf = (seq_t *)calloc(cap + 1, sizeof(seq_t)); /* +1 for 1‑based indexing */
        if (!buf) abort();             /* allocation failed */
        for (idx_t i = 1; i <= 16; ++i)
            buf[i] = init[i];
        last = 16;
    }

    /* Grow buffer if required. */
    if (n > cap) {
        idx_t newcap = cap;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(buf, (newcap + 1) * sizeof(seq_t));
        if (!tmp) abort();
        buf = tmp;
        cap = newcap;
    }

    /* Return immediately if already computed. */
    if (n <= last) return buf[n];

    /* Compute missing terms using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i)
    {
        /* sum of positive contributions */
        seq_t pos = 0;
        pos += mul_const(buf[i - 2], 6);
        pos += mul_const(buf[i - 3], 3);
        pos += mul_const(buf[i - 4], 4);
        pos += mul_const(buf[i - 6], 2);

        /* sum of negative contributions */
        seq_t neg = 0;
        neg += mul_const(buf[i - 5], 2);
        neg += mul_const(buf[i - 7], 7);
        neg += mul_const(buf[i - 8], 19);
        neg += mul_const(buf[i - 9], 23);
        neg += mul_const(buf[i -10], 19);
        neg += mul_const(buf[i -11], 19);
        neg += mul_const(buf[i -12], 4);
        neg += mul_const(buf[i -13], 4);

        buf[i] = pos - neg;    /* safe: pos ≥ neg for all valid n */
    }
    last = n;
    return buf[n];
}
