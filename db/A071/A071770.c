/*
 * A071770  Tersum n + [n/3] (answer recorded in base 10).
 * https://oeis.org/A071770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ternary (base‑3) sum of n and floor(n/3), result
 *            interpreted in base 10
 * Range:     u8 n <= 242, u16 n <= 8277, u32 n <= 8277, u64 n <= 8277,
 *            u128 n <= 8277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res  = 0;          /* result in decimal (base 10) */
    seq_t pow3 = 1;          /* powers of 3 in base 10 */

    idx_t nn = n;            /* copy of n */
    idx_t mm = n / 3;        /* floor(n/3) */

    /* Add the base‑3 digits of nn and mm without carry (mod 3). */
    while (nn > 0 || mm > 0) {
        seq_t dn = nn % 3;          /* least significant digit of n  (base 3) */
        seq_t dm = mm % 3;          /* least significant digit of m  (base 3) */
        seq_t dr = (dn + dm) % 3;   /* digit of the ternary sum */
        res += dr * pow3;           /* accumulate the current digit in base 10 */
        pow3 *= 3;                  /* move to next base‑3 place */
        nn /= 3;
        mm /= 3;
    }

    return res;
}
