/*
 * A074999  Floor[concatenation of nine consecutive numbers from n to n+8
 *          divided by 9].
 * https://oeis.org/A074999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = floor(concatenation of n, n+1, …, n+8 divided by 9)
 * Range:     u8 none, u16 none, u32 n <= 3, u64 n <= 93, u128 n <= 1000,
 *            big any n
 * Helpers:   idx_digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/idx_digits.h"

seq_t a(idx_t n)
{
    seq_t q = 0;          /* current quotient floor(concat/9)          */
    unsigned r = 0;       /* current remainder mod 9 (0..8)           */

    for (idx_t k = 0; k < 9; ++k) {
        idx_t cur = n + k;                 /* current number to append */
        unsigned d   = idx_digits(cur);    /* number of decimal digits of cur */
        seq_t  base = pow10(d);            /* 10^d */

        /* t = r * base + cur  (value < 9*1000 + 999 < 9000) */
        seq_t t = (seq_t)r * base + (seq_t)cur;

        r = (unsigned)(t % 9);             /* new remainder          */
        q = q * base + (t / 9);             /* new quotient          */
    }

    return q;
}
