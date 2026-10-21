/*
 * A085424  Number of ones in the symmetric signed digit expansion of n with
 *          q=2 (i.e., the representation of n in the (-1,0,1)_2 number
 *          system).
 * https://oeis.org/A085424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of +1 digits in the non‑adjacent form (NAF)
 *            expansion of n with signed binary digits {-1,0,1}. The NAF is
 *            constructed by repeatedly: if n is odd, set di = 2 - (n mod 4)
 *            (di = +1 for n≡1 (mod 4), di = -1 for n≡3 (mod 4)) else di = 0.
 *            n ← (n - di) / 2. The function counts the +1 digits
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ones = 0;        /* number of +1 digits encountered */
    idx_t x = n;           /* working copy of the input */

    while (x != 0) {
        if (x & 1) {       /* x is odd */
            idx_t mod4 = x & 3;   /* x % 4, 1 or 3 for odd x */
            if (mod4 == 1) {      /* choose +1 */
                ones += 1;         /* count the positive one */
                x = (x - 1) >> 1;  /* remove the +1 and shift */
            } else {              /* mod4 == 3, choose -1 */
                x = (x + 1) >> 1;  /* add 1 (i.e., subtract -1) and shift */
            }
        } else {              /* even, digit is 0 */
            x >>= 1;
        }
    }

    return ones;
}
