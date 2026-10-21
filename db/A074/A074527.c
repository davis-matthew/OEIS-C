/*
 * A074527  a(n) = 2^n + 3^n + 5^n.
 * https://oeis.org/A074527
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 3^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use only integer arithmetic; seq_mul is the only helper needed
 * for arbitrary‑precision multiplication.  Addition is performed with
 * the native + operator, which is defined for all seq_t types. */
#include <stdbool.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* base values as seq_t */
    const seq_t two   = (seq_t)2;
    const seq_t three = (seq_t)3;
    const seq_t five  = (seq_t)5;
    const seq_t one   = (seq_t)1;

    /* fast exponentiation by squaring for each base */
    seq_t pow2 = two, pow3 = three, pow5 = five;
    seq_t res2 = one, res3 = one, res5 = one;
    idx_t e = n;

    while (e != 0) {
        if (e & 1) {
            res2 = seq_mul(res2, pow2);
            res3 = seq_mul(res3, pow3);
            res5 = seq_mul(res5, pow5);
        }
        pow2 = seq_mul(pow2, pow2);
        pow3 = seq_mul(pow3, pow3);
        pow5 = seq_mul(pow5, pow5);
        e >>= 1;
    }

    /* sum the three powers */
    seq_t sum12 = res2 + res3;
    seq_t sum   = sum12 + res5;
    return sum;
}
