/*
 * A118640  Result of left concatenation of the next Roman-numeral symbol.
 * https://oeis.org/A118640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base, fini, full
 * Formula:   a(n) = if n even: 6*(10^(n/2) - 1)/9 * if n odd: 10^((n-1)/2) +
 *            6*(10^((n-1)/2)-1)/9
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not used but safe */
    if ((n & 1) == 0) {                       /* n even */
        idx_t len = n / 2;                    /* number of digits */
        seq_t v = pow10(len);                /* 10^len */
        seq_t r = v - 1;                     /* 10^len - 1 */
        r = r / 9;                           /* /(9)  */
        r = r * 6;                           /* *6    */
        return r;                            /* 6*(10^len-1)/9 */
    } else {                                  /* n odd */
        idx_t len = (n + 1) / 2;             /* number of digits */
        seq_t v = pow10(len - 1);            /* 10^(len-1) */
        seq_t r = v - 1;                     /* 10^(len-1) - 1 */
        r = r / 9;                           /* /(9) */
        r = r * 6;                           /* *6  */
        return v + r;                        /* 10^(len-1) + 6*(10^(len-1)-1)/9 */
    }
}
