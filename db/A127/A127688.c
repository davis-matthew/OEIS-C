/*
 * A127688  a(n) = (n+1)^n - n!!.
 * https://oeis.org/A127688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^n - n!! (OEIS A127688)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"   /* seq_t pow_seq(seq_t base, int exp) */
#include "../utils/seq_mul.h"   /* seq_t seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* compute (n+1)^n */
    seq_t pow_val = pow_seq((seq_t)(n + 1), (int)n);

    /* compute n!! (double factorial) */
    seq_t double_fact = (seq_t)1;
    for (idx_t i = n; i > 0; ) {
        double_fact = seq_mul(double_fact, (seq_t)i);
        if (i <= 2) break;              /* finish with 1 or 2 */
        i -= 2;
    }

    /* return the difference */
    return pow_val - double_fact;
}
