/*
 * A238291  Final digit of real part of (n+n*i)^n.
 * https://oeis.org/A238291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = last digit of the real part of (n+n*i)^n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   powmod10_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/powmod10_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety; offset starts at 1 */

    unsigned char base = powmod10_uint(n, n);   /* n^n (mod 10) */

    unsigned char an;
    if ((n & 1) == 0) {            /* n even */
        if ((n % 4) == 0) {
            an = 6;                /* for n≥4, n≡0 (mod4): a_n≡6 (mod10) */
        } else {                   /* n ≡2 (mod4) */
            an = 0;
        }
    } else {                       /* n odd */
        int mod8 = n % 8;
        int positive = (mod8 == 1 || mod8 == 7);   /* sign of a_n */
        idx_t k = (n - 1) >> 1;                    /* (n-1)/2 */
        unsigned char pow2 = powmod10_uint(2, k);   /* 2^k (mod10) */
        if (!positive) {
            pow2 = (10 - pow2) % 10;                /* negative sign */
        }
        an = pow2;
    }

    unsigned char res = (base * an) % 10;
    return (seq_t)res;
}
