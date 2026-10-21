/*
 * A224438  The hyper-Wiener index of the nanostar dendrimer NS_1[n], defined
 *          pictorially in the A. R. Ashrafi et al. reference.
 * https://oeis.org/A224438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -7487 + (-59832 - 10740*n + 4200*n*n)*2^n + (69402 -
 *            36480*n + 14400*n*n)*4^n
 * Range:     u8 none, u16 n <= 1, u32 n <= 6, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 2^n and 4^n */
    seq_t pow2 = 1;
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
        pow4 *= (seq_t)4;
    }

    /* coefficients (may be negative) */
    int64_t c2 = 4200LL * (int64_t)n * (int64_t)n
               - 10740LL * (int64_t)n
               - 59832LL;
    int64_t c3 = 14400LL * (int64_t)n * (int64_t)n
               - 36480LL * (int64_t)n
               + 69402LL;

    /* absolute values for multiplication */
    seq_t abs_c2 = (c2 >= 0) ? (seq_t)c2 : (seq_t)(-c2);
    seq_t abs_c3 = (c3 >= 0) ? (seq_t)c3 : (seq_t)(-c3);

    /* value contributions */
    seq_t prod2 = pow2 * abs_c2;
    seq_t prod3 = pow4 * abs_c3;

    seq_t result = 0;
    if (c2 >= 0) result += prod2; else result -= prod2;
    if (c3 >= 0) result += prod3; else result -= prod3;

    /* subtract constant offset */
    result -= (seq_t)7487;

    return result;
}
