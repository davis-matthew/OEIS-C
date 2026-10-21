/*
 * A067534  a(n) = 4^n * Sum_{i=1..n} i^4/4^i.
 * https://oeis.org/A067534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (380*4^n - 27*n^4 - 144*n^3 - 360*n^2 - 528*n - 380) / 81
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) /* offset starts at 1; return 0 for non‑valid indices */
        return 0;

    /* 4^n as a big integer */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);

    /* n, n^2, n^3, n^4 as big integers */
    seq_t n_seq = (seq_t)n;
    seq_t n2    = n_seq * n_seq;           /* n^2   */
    seq_t n3    = n2 * n_seq;              /* n^3   */
    seq_t n4    = n3 * n_seq;              /* n^4   */

    /* 27*n^4 + 144*n^3 + 360*n^2 + 528*n + 380   (all terms as seq_t) */
    seq_t poly = (seq_t)27 * n4
               + (seq_t)144 * n3
               + (seq_t)360 * n2
               + (seq_t)528 * n_seq
               + (seq_t)380;

    /* Numerator: 380*4^n - (27*n^4 + 144*n^3 + 360*n^2 + 528*n + 380) */
    seq_t num = (seq_t)380 * pow4 - poly;

    /* Division by 81 – guaranteed to be integral */
    return num / (seq_t)81;
}
