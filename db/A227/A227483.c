/*
 * A227483  The hyper-Wiener index of the nanostar dendrimer defined
 *          pictorially as NS[n] in the M. Mirzargar reference.
 * https://oeis.org/A227483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 116340 - 2^n*3768423/2 + 4^n*(455625*n^2 - 2112075*n/2 +
 *            3582801/2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Powers of two */
    seq_t pow2 = pow_uint(2, n);
    seq_t pow4 = pow_uint(4, n);

    /* Convert index to seq_t once */
    seq_t n_seq = (seq_t)n;

    /* Polynomial numerator: 2*455625*n^2 - 2112075*n + 3582801 */
    seq_t term1 = n_seq * n_seq * 911250u;       /* 911250 = 2*455625 */
    seq_t term2 = n_seq * 2112075u;
    seq_t poly   = term1 - term2 + 3582801u;

    /* Compute the two main terms */
    seq_t T1 = pow2 * 3768423u;
    seq_t T2 = pow4 * poly;

    /* Combine everything */
    seq_t numer = 232680u;     /* 2 * 116340 */
    numer -= T1;
    numer += T2;

    /* Final result */
    return numer / 2u;
}
