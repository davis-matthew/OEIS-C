/*
 * A221007  The hyper-Wiener index of the nanostar dendrimer NS[n], defined
 *          pictorially in the Karbasioun-Ashrafi-Diudea reference.
 * https://oeis.org/A221007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1095-2^n*28971+4^n*(11664*n^2-9720*n+32346)
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                 /* convert n to seq_t */
    seq_t pow2n = pow2(n);               /* 2^n   */
    seq_t pow4n = pow4(n);               /* 4^n   */
    seq_t poly = (seq_t)11664 * (nn * nn) - (seq_t)9720 * nn + (seq_t)32346;
    seq_t term3 = pow4n * poly;          /* 4^n * (poly) */
    seq_t term2 = pow2n * (seq_t)28971;  /* 2^n * 28971 */
    seq_t result = term3 - term2 + (seq_t)1095; /* 1095 - term2 + term3 */
    return result;
}
