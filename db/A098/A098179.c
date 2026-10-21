/*
 * A098179  Expansion of (1-3*x+3*x^2)/(1-5*x+10*x^2-10*x^3+4*x^4).
 * https://oeis.org/A098179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1)-10*a(n-2)+10*a(n-3)-4*a(n-4); a(0)=1, a(1)=2,
 *            a(2)=3, a(3)=5. Equivalent closed form: a(n) = 2^n +
 *            2^(n/2)*cos(pi*n/4) - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t val = pow2(n) - 1;       /* 2^n - 1  */

    /* Add/subtract the second term depending on n mod 8. */
    switch (n % 8) {
        case 0:  /* +2^(n/2) */
            val += pow2(n / 2);
            break;
        case 1:  /* +2^((n-1)/2) */
            val += pow2((n - 1) / 2);
            break;
        case 2: /* 0 */
            break;
        case 3: /* -2^((n-1)/2) */
            val -= pow2((n - 1) / 2);
            break;
        case 4: /* -2^(n/2) */
            val -= pow2(n / 2);
            break;
        case 5: /* -2^((n-1)/2) */
            val -= pow2((n - 1) / 2);
            break;
        case 6: /* 0 */
            break;
        case 7: /* +2^((n-1)/2) */
            val += pow2((n - 1) / 2);
            break;
    }
    return val;
}
