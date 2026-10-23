/*
 * A137878  Perfect squares among 17-gonal numbers A051869(k) = k*(15*k -
 *          13)/2.
 * https://oeis.org/A137878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A137878: perfect squares among 17‑gonal numbers
   Formula: a(n) = A137878(n)
   Recurrence (for n > 5):
     a(n) = a(n-1) + 232322*a(n-2) - 232322*a(n-3) - a(n-4) + a(n-5)
   Initial values:
     a(1)=1
     a(2)=17689
     a(3)=378225
     a(4)=4109707449
     a(5)=87870152041
*/

#include <stdint.h>

seq_t a(idx_t n) {
    /* constants for the first 5 terms */
    static const seq_t c1 = (seq_t)1ULL;
    static const seq_t c2 = (seq_t)17689ULL;
    static const seq_t c3 = (seq_t)378225ULL;
    static const seq_t c4 = (seq_t)4109707449ULL;
    static const seq_t c5 = (seq_t)87870152041ULL;

    if (n == 0) return (seq_t)0ULL;            /* not used, but harmless */
    if (n == 1) return c1;
    if (n == 2) return c2;
    if (n == 3) return c3;
    if (n == 4) return c4;
    if (n == 5) return c5;

    /* Iteratively compute the sequence using the recurrence,
       keeping only the last five terms. */
    seq_t a_nm5 = c1;      /* a(n-5) */
    seq_t a_nm4 = c2;      /* a(n-4) */
    seq_t a_nm3 = c3;      /* a(n-3) */
    seq_t a_nm2 = c4;      /* a(n-2) */
    seq_t a_nm1 = c5;      /* a(n-1) */
    seq_t a_n   = c5;      /* placeholder for current term */

    const seq_t mult = (seq_t)232322ULL;   /* multiplier used in recurrence */

    for (idx_t i = 6; i <= n; ++i) {
        /* a(n) = a(n-1) + mult*a(n-2) - mult*a(n-3) - a(n-4) + a(n-5) */
        a_n = a_nm1 + mult * a_nm2;
        a_n -= mult * a_nm3;
        a_n -= a_nm4;
        a_n += a_nm5;

        /* shift the window forward */
        a_nm5 = a_nm4;
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_n;
}
