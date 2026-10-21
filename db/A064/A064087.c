/*
 * A064087  Generalized Catalan numbers C(4; n).
 * https://oeis.org/A064087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Main recurrence: 3*n*a(n) + (-47*n+72)*a(n-1) + 8*(-2*n+3)*a(n-2) = 0
   with a(0) = 1, a(1) = 1.
   Equivalent: a(n) = ((47*n-72)*a(n-1) + (16*n-24)*a(n-2)) / (3*n). */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* previous two terms */
    seq_t a_nm2 = (seq_t)1; /* a(n-2) */
    seq_t a_nm1 = (seq_t)1; /* a(n-1) */
    seq_t a_n   = 0;

    /* iterate from 2 up to n */
    for (idx_t i = 2; i <= n; ++i) {
        /* use 128‑bit arithmetic for the intermediate products */
        unsigned __int128 coeff1 = (unsigned __int128)47 * i - 72;
        unsigned __int128 coeff2 = (unsigned __int128)16 * i - 24;

        unsigned __int128 prod1 = coeff1 * (unsigned __int128)a_nm1;
        unsigned __int128 prod2 = coeff2 * (unsigned __int128)a_nm2;
        unsigned __int128 num   = prod1 + prod2;
        unsigned __int128 denom = (unsigned __int128)3 * i;

        a_n = (seq_t)(num / denom);  /* division is exact by the recurrence */

        /* shift for next step */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
