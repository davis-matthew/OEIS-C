/*
 * A119810  Partial quotients of the continued fraction of the constant
 *          defined by binary sums involving Beatty sequences: c = Sum_{n>=1}
 *          1/2^A049472(n) = Sum_{n>=1} A001951(n)/2^n.
 * https://oeis.org/A119810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn
 * Formula:   a(n) = 2^A001333(n-1) + 2^A000129(n-2) for n>1, with a(1)=2.
 *            A001333: 1,3,7,17,41,99,... (recurrence: w(1)=1, w(2)=3,
 *            w(k)=2*w(k-1)+w(k-2)) A000129: Pell numbers P(0)=0, P(1)=1,
 *            P(k)=2*P(k-1)+P(k-2) The function is O(n) and works with any
 *            seq_t/idx_t sizes, using the provided big_pow helper to co...
 * Range:     u8 n <= 4, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Base case: a(1) = 2 */
    if (n == 1)
        return (seq_t)2;

    /* Compute A001333(n-1) → w */
    seq_t w;
    if (n-1 == 1)
        w = (seq_t)1;  /* w(1) */
    else if (n-1 == 2)
        w = (seq_t)3;  /* w(2) */
    else {
        seq_t w_prev = (seq_t)1;  /* w(1) */
        seq_t w_cur  = (seq_t)3;  /* w(2) */
        for (idx_t i = 3; i <= n-1; ++i) {
            seq_t w_next = (seq_t)2 * w_cur + w_prev;
            w_prev = w_cur;
            w_cur  = w_next;
        }
        w = w_cur;
    }

    /* Compute A000129(n-2) → p (Pell number) */
    idx_t p_index = n-2;
    seq_t p;
    if (p_index == 0)
        p = (seq_t)0;  /* P(0) */
    else if (p_index == 1)
        p = (seq_t)1;  /* P(1) */
    else {
        seq_t p_prev = (seq_t)0;  /* P(0) */
        seq_t p_cur  = (seq_t)1;  /* P(1) */
        for (idx_t i = 2; i <= p_index; ++i) {
            seq_t p_next = (seq_t)2 * p_cur + p_prev;
            p_prev = p_cur;
            p_cur  = p_next;
        }
        p = p_cur;
    }

    /* Compute 2^w + 2^p using big_pow (supports large exponents) */
    seq_t term1 = big_pow((seq_t)2, w);
    seq_t term2 = big_pow((seq_t)2, p);
    return term1 + term2;
}
