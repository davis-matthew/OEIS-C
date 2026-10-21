/*
 * A221009  The hyper-Wiener index of the nanostar dendrimer D_3[n] , defined
 *          pictorially in the Alikhani - Iranmanesh reference.
 * https://oeis.org/A221009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9531-2^n*168276+4^n*(44100*n^2-92190*n+162138)
 * Range:     u8 none, u16 n <= 0, u32 n <= 5, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic */
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t n_seq = (seq_t)n;

    /* Polynomial part: 44100*n^2 - 92190*n + 162138 */
    seq_t poly = (seq_t)44100 * nsq
               - (seq_t)92190 * n_seq
               + (seq_t)162138;

    /* Powers of 2 and 4 */
    seq_t pow2n = pow2(n);
    seq_t pow4n = pow4(n);

    /* Compute 4^n * poly */
    seq_t term1 = pow4n * poly;

    /* Compute 2^n * 168276 */
    seq_t term2 = pow2n * (seq_t)168276;

    /* Final result: 9531 - term2 + term1 */
    seq_t result = term1 - term2 + (seq_t)9531;

    return result;
}
