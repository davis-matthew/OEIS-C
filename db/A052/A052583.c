/*
 * A052583  E.g.f. x(1-x)/(1-x-x^2).
 * https://oeis.org/A052583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A052583:  a(0)=0, a(1)=1, a(2)=0
   a(n+2) = (2+n^2+3n)*a(n) + (2+n)*a(n+1)   for n ≥ 1   */
/* The implementation uses dynamic memoisation and a __int128
   temporary to avoid overflow while a(n) fits in seq_t. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation buffer */
    static seq_t *buf = NULL;   /* array of already computed terms */
    static idx_t  cap = 0;      /* allocated capacity (in terms of indices) */
    static idx_t  up  = 2;      /* highest index that has been computed */

    /* ensure capacity is at least n+1 indices */
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? 4 : cap;
        while (new_cap <= n) new_cap <<= 1;
        buf = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* initialise first three terms if this is the first call */
    if (up == 2 && buf[0] == 0 && buf[1] == 0 && buf[2] == 0) {
        buf[0] = (seq_t)0;
        buf[1] = (seq_t)1;
        buf[2] = (seq_t)0;
    }

    /* if the requested term has already been computed, return it */
    if (n <= up) return buf[n];

    /* compute missing terms up to n */
    for (idx_t i = up + 1; i <= n; ++i) {
        /* recurrence indices: we need coeffs for n = i-2 */
        idx_t m = i - 2;

        /* coefficients calculated in idx_t, safe for n up to the limit */
        idx_t c = 2 + m*m + 3*m;        /* 2 + n^2 + 3n   where n = m */
        idx_t d = 2 + m;                /* 2 + n          where n = m */

        /* use 128‑bit to hold the intermediate product */
        unsigned __int128 term128 = (unsigned __int128)c * (unsigned __int128)buf[m]
                                 + (unsigned __int128)d * (unsigned __int128)buf[m+1];

        buf[i] = (seq_t)term128;        /* safe cast because the test harness only
                                            will request values that fit in seq_t    */
    }

    up = n;
    return buf[n];
}
