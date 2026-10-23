/*
 * A127427  a(n) = v_3( (6n)! ) - v_3( (3n)! ), where v_3(N) is the 3-adic
 *          valuation A007949(N).
 * https://oeis.org/A127427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (3*n + S₃(3*n) - S₃(6*n)) / 2 where S₃(x) is the sum of
 *            the base‑3 digits of x. This equals the 3‑adic valuation
 *            difference v₃((6n)!) - v₃((3n)!). Execution time is O(log₃ n)
 * Range:     u8 n <= 169, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sum_base3.h"

seq_t a(idx_t n)
{
    idx_t n3 = n * (idx_t)3;
    idx_t n6 = n * (idx_t)6;

    seq_t s3 = sum_base3(n3);
    seq_t s6 = sum_base3(n6);

    seq_t temp = (seq_t)3 * (seq_t)n + s3;
    temp -= s6;           /* guaranteed non‑negative for all n */
    return temp / (seq_t)2;
}
