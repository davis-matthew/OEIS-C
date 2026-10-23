/*
 * A100979  Number of totally ramified extensions over Q_2 with degree n in
 *          the algebraic closure of Q_2.
 * https://oeis.org/A100979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * sum_{s=0..m} 2^s * (2^{n - n/2^s} - 2^{n -
 *            n/2^{s-1}}), where m = v_2(n) (the exponent of the highest power
 *            of 2 dividing n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   power_of_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/power_of_two.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 (not part of the sequence). */
    if (n == 0) return 0;

    /* Determine m = v2(n). */
    idx_t m = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++m;
    }

    seq_t sum = 0;

    for (idx_t s = 0; s <= m; ++s) {
        seq_t term;
        if (s == 0) {
            /* For s = 0 the inner difference is 1 - 0 = 1. */
            term = 1;
        } else {
            idx_t pow_s = 1ULL << s;           /* 2^s  */
            idx_t div1  = n / pow_s;           /* n / 2^s */
            idx_t div2  = n / (pow_s >> 1);    /* n / 2^{s-1} */

            idx_t exp1 = n - div1;             /* n - n/2^s */
            idx_t exp2 = n - div2;             /* n - n/2^{s-1} */

            seq_t a1 = power_of_two(exp1);
            seq_t a2 = power_of_two(exp2);

            term = ((seq_t)pow_s) * (a1 - a2);
        }
        sum += term;
    }

    return ((seq_t)n) * sum;
}
