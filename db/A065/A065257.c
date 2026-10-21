/*
 * A065257  Quintal Queens permutation of N: double (mod 5) each digit (0->0,
 *          1->2, 2->4, 3->1, 4->3) of the base-5 representation of n-1, add
 *          one.
 * https://oeis.org/A065257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is obtained by writing n‑1 in base‑5, multiplying each
 *            base‑5 digit by 2 modulo 5 (0→0, 1→2, 2→4, 3→1, 4→3),
 *            interpreting the resulting digits as a base‑5 number, and adding
 *            1
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* map[d] = (2*d) mod 5 */
    static const uint8_t map[5] = {0, 2, 4, 1, 3};

    idx_t x = n - 1;           /* n is 1‑based, so subtract 1 */
    seq_t val = 0;             /* value of the transformed number */
    seq_t pow5 = 1;            /* current power of 5 */

    if (x == 0) {              /* n == 1: special case, loop will skip */
        val = 0;
    } else {
        /* process all base‑5 digits of x */
        while (x != 0) {
            uint8_t d = (uint8_t)(x % 5);                 /* current digit of x */
            seq_t mapped = (seq_t)map[d];                 /* digit after doubling mod 5 */
            val += mapped * pow5;                         /* add contribution to value */
            pow5 *= 5;                                    /* next power of 5 */
            x /= 5;                                       /* move to next digit */
        }
    }

    val += 1;                     /* final +1 as per sequence definition */
    return val;
}
