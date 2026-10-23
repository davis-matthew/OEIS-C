/*
 * A190416  Number of (n+2)X(n+2) symmetric binary matrices without the
 *          pattern 1 1 1 antidiagonally.
 * https://oeis.org/A190416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{L=1}^{k-1} f(L)^2 · f(k), where k = n+2 and f(L) is
 *            the number of palindromic binary strings of length L that avoid
 *            the substring 111. Equivalently, let m = ceil(L/2); then f(L) is
 *            the number of binary strings of length m that avoid 111 and do
 *            not end with “11”
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* size of the matrix */
    const idx_t k = n + (idx_t)2;

    /* maximum half-length needed */
    const idx_t maxM = (k + (idx_t)1) / (idx_t)2;

    /* allocate array for counts of length m */
    seq_t *countLen = (seq_t *)calloc((size_t)(maxM + (idx_t)1), sizeof(seq_t));
    if (!countLen) return (seq_t)0;   /* just for safety */

    /* base cases */
    countLen[1] = (seq_t)2;          /* strings of length 1, any bit */

    if (maxM >= 2) {
        /* DP variables for strings of length l (l >= 2) avoiding 111.
           States encode last two bits:
           0: 00, 1: 01, 2: 10, 3: 11
        */
        seq_t a0 = (seq_t)1, a1 = (seq_t)1, a2 = (seq_t)1, a3 = (seq_t)1; /* l = 2 */
        /* total strings of length 2 avoiding 111 */
        seq_t total_no = a0 + a1 + a2 + a3;          /* = 4 */
        seq_t end_11   = a3;                         /* = 1 */
        countLen[2] = total_no - end_11;             /* = 3 */

        /* iterate for l = 3 .. maxM */
        for (idx_t l = (idx_t)3; l <= maxM; ++l) {
            seq_t new0 = a0 + a2;
            seq_t new1 = a0 + a2;
            seq_t new2 = a1 + a3;
            seq_t new3 = a1;                   /* 3 cannot be produced from 3 */

            seq_t total = new0 + new1 + new2 + new3;
            seq_t end11 = new3;

            countLen[l] = total - end11;

            /* prepare for next iteration */
            a0 = new0; a1 = new1; a2 = new2; a3 = new3;
        }
    }

    /* Compute the product */
    seq_t result = (seq_t)1;
    for (idx_t L = (idx_t)1; L <= k - (idx_t)1; ++L) {
        idx_t m = (L + (idx_t)1) / (idx_t)2;   /* ceil(L/2) */
        seq_t f = countLen[m];
        /* square the factor f(L) */
        result = result * f;
        result = result * f;
    }
    /* final factor f(k) */
    idx_t mk = (k + (idx_t)1) / (idx_t)2;      /* ceil(k/2) */
    result = result * countLen[mk];

    free(countLen);
    return result;
}
