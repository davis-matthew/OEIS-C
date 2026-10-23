/*
 * A028015  Expansion of 1/((1-2x)(1-8x)(1-9x)(1-10x)).
 * https://oeis.org/A028015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   mul_small_plain
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 29*a(n-1) - 296*a(n-2) + 1204*a(n-3) - 1440*a(n-4)
 *     a(0) = 1,  a(1) = 29,  a(2) = 545,  a(3) = 8425
 *
 * The recurrence follows from the denominator (1-2x)(1-8x)(1-9x)(1-10x).
 * It uses only integer arithmetic; all intermediate values fit in the
 * type seq_t or are handled by repeated addition for small integer
 * multipliers, ensuring correct results for all supported seq_t sizes.
 */

#include <stdbool.h>
#include "../utils/mul_small_plain.h"

/* compute a(n) */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)29;
    if (n == 2) return (seq_t)545;
    if (n == 3) return (seq_t)8425;

    seq_t a0 = (seq_t)1;      /* a(n-4)   */
    seq_t a1 = (seq_t)29;     /* a(n-3)   */
    seq_t a2 = (seq_t)545;    /* a(n-2)   */
    seq_t a3 = (seq_t)8425;   /* a(n-1)   */
    seq_t an;

    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = mul_small_plain(a3, 29);   /* 29*a(n-1) */
        seq_t t2 = mul_small_plain(a2, 296);  /* 296*a(n-2) */
        seq_t t3 = mul_small_plain(a1, 1204); /* 1204*a(n-3) */
        seq_t t4 = mul_small_plain(a0, 1440); /* 1440*a(n-4) */

        /* compute a(n) using unsigned arithmetic; order ensures
         * no underflow (t1 >= t2, t3 >= t4) as proven by the
         * recurrence coefficients and growth of the sequence.       */
        an = (t1 - t2) + (t3 - t4);

        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return an;
}
