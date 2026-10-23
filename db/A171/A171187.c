/*
 * A171187  a(n) = Sum_{k=0..[n/2]} A034807(n,k)^n, where A034807 is a
 *          triangle of Lucas polynomials.
 * https://oeis.org/A171187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} (C(n‑k,k)+C(n‑k‑1,k‑1))^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t lim = n / 2;
    seq_t sum  = 0;
    seq_t a_val = 1;            /* C(n,0)    */
    seq_t b_val = 0;            /* C(n-1,-1) = 0 */
    seq_t sn   = (seq_t)n;      /* exponent is n */

    for (idx_t k = 0; k <= lim; ++k)
    {
        /* value to raise to the n‑th power */
        seq_t val = a_val + b_val;
        sum += big_pow(val, sn);

        if (k == lim) break;  /* no next term */

        /* next a_val : C(n‑(k+1), k+1) */
        seq_t a_num = a_val *
                      (seq_t)(n - 2 * k) *
                      (seq_t)(n - 2 * k - 1);
        seq_t a_den = (seq_t)(k + 1) * (seq_t)(n - k);
        a_val = a_num / a_den;      /* exact division */

        /* next b_val : C(n‑(k+1)-1, (k+1)-1) */
        if (k == 0)
            b_val = 1;              /* C(n‑2,0) = 1 */
        else
        {
            seq_t b_num = b_val *
                          (seq_t)(n - 2 * k) *
                          (seq_t)(n - 2 * k - 1);
            seq_t b_den = (seq_t)k * (seq_t)(n - k - 1);
            b_val = b_num / b_den;  /* exact division */
        }
    }
    return sum;
}
