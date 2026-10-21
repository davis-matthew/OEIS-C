/*
 * A221004  The Wiener index of the nanostar dendrimer NS[n], defined
 *          pictorially in the Wang-Hua reference.
 * https://oeis.org/A221004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 627 + 2^n*(936*n+1602)+4^n*(432*n-792)
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t pow2n = pow2(n);
    seq_t pow4n = pow4(n);

    seq_t term = (seq_t)627;
    term += pow2n * ((seq_t)936 * sn + (seq_t)1602);
    term += pow4n * ((seq_t)432 * sn);
    term -= pow4n * (seq_t)792;

    return term;
}
