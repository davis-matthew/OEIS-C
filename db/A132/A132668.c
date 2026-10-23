/*
 * A132668  a(1)=1, a(n) = 4*a(n-1) if the minimal positive integer not yet in
 *          the sequence is greater than a(n-1), else a(n) = a(n-1) - 1.
 * https://oeis.org/A132668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^⌊1 + (k + 1)/2⌋ + 7·4^⌊k/2⌋ – 5)/3 – n, where k = r if
 *            r is odd, otherwise k = s; r = ceil(2·log₄((3n+4)/7)), s =
 *            ceil(2·log₄((3n+4)/8)). The implementation follows these
 *            inequalities using only integer arithmetic
 * Range:     u8 n <= 36, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow4.h"   /* provides seq_t pow4(idx_t exp) */

/* seq_t and idx_t are supplied by the build system */
seq_t a(idx_t n)
{
    /* Compute target = 3*n + 4 as seq_t */
    seq_t target = (seq_t)3 * n + 4;

    /* Find minimal r such that 7·2^r >= target */
    idx_t r = 0;
    seq_t val = 7;
    while (val < target) {
        val *= (seq_t)2;
        ++r;
    }

    /* Find minimal s such that 8·2^s >= target */
    idx_t s = 0;
    val = 8;
    while (val < target) {
        val *= (seq_t)2;
        ++s;
    }

    /* Choose k according to parity of r */
    idx_t k = (r & 1) ? r : s;

    /* Compute exponents for the two 4-powers */
    idx_t e1, e2;
    if ((k & 1) == 0) {           /* k even */
        e1 = (k >> 1) + 1;        /* floor(k/2) + 1 */
        e2 = k >> 1;              /* floor(k/2) */
    } else {                       /* k odd */
        e1 = ((k + 1) >> 1) + 1;  /* (k+1)/2 + 1 */
        e2 = (k - 1) >> 1;        /* (k-1)/2 */
    }

    /* Evaluate the powers of 4 */
    seq_t pow1 = pow4(e1);
    seq_t pow2 = pow4(e2);

    /* Compute (pow1 + 7*pow2 - 5) / 3 - n */
    seq_t tmp = pow1 + (seq_t)7 * pow2;
    tmp -= 5;
    seq_t result = tmp / 3;
    result -= n;
    return result;
}
