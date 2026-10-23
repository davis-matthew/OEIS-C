/*
 * A033763  Product t2(q^d); d | 4, where t2 = theta2(q)/(2*q^(1/4)).
 * https://oeis.org/A033763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive odd solutions to a^2 + 2*b^2 + 4*c^2 =
 *            8*n + 7
 * Range:     u8 n <= 24, u16 n <= 8716, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Valuation target */
    seq_t maxVal = ((seq_t)n) * 8 + 7;
    seq_t count = 0;

    for (idx_t a = 1; ; a += 2) {                 /* odd a >=1 */
        seq_t aa = (seq_t)a * a;
        if (aa > maxVal - 6) break;              /* min rest 2*1^2+4*1^2 = 6 */

        for (idx_t b = 1; ; b += 2) {             /* odd b >=1 */
            seq_t bb = (seq_t)b * b;
            seq_t subtotal = aa + (seq_t)2 * bb;   /* a^2 + 2*b^2 */

            if (subtotal > maxVal - 4) break;     /* min rest 4*1^2 = 4 */

            for (idx_t c = 1; ; c += 2) {         /* odd c >=1 */
                seq_t cc2 = (seq_t)c * c;
                seq_t val = subtotal + (seq_t)4 * cc2;

                if (val > maxVal) break;          /* exceed target */
                if (val == maxVal) count++;       /* perfect match */
            }
        }
    }

    return count;
}
