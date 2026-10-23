/*
 * A021434  Expansion of 1/((1-x)(1-3x)(1-5x)(1-8x)).
 * https://oeis.org/A021434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A021434:
   a(0) = 1, a(1) = 17, a(2) = 194, a(3) = 1882;
   a(4) = 17*a(3) - 95*a(2) + 199*a(1) - 120*a(0);
   a(5) = 143835;
   for n >= 6:
       a(n) = 17*a(n-1) - 95*a(n-2) + 199*a(n-3) - 120*a(n-4);
   (All operations are performed in type seq_t.  For seq_t = uint64_t
   the sequence is correct for all terms that fit.) */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)17;
    if (n == 2) return (seq_t)194;
    if (n == 3) return (seq_t)1882;

    /* a(4) via the recurrence */
    seq_t a4 = 17u * (seq_t)1882
             - 95u * (seq_t)194
             + 199u * (seq_t)17
             - 120u * (seq_t)1;

    if (n == 4) return a4;

    /* a(5) from the closed form (or constant) */
    seq_t a5 = (seq_t)143835;

    if (n == 5) return a5;

    /* iterate from n = 6 upwards using the 4‑term recurrence */
    seq_t a_nm4 = (seq_t)194;   /* a(n-4) when i = 6 */
    seq_t a_nm3 = (seq_t)1882;  /* a(n-3) */
    seq_t a_nm2 = a4;           /* a(n-2) */
    seq_t a_nm1 = a5;           /* a(n-1) */
    seq_t an;

    for (idx_t i = 6; i <= n; ++i) {
        /* compute a(i) */
        an = 17u * a_nm1 + 199u * a_nm3;
        an -= 95u * a_nm2;
        an -= 120u * a_nm4;

        /* shift window for next step */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }
    return a_nm1;
}
