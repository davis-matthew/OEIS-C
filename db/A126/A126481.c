/*
 * A126481  Number of base 13 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of length‑n strings over digits 0..12 * with |d_i
 *            - d_{i+1}| ≤ 3. * DP recurrence: * let b_k(j) = number of
 *            length‑k strings ending with digit j * b_1(j) = 1 for all
 *            j∈[0,12] * b_{k+1}(j) = Σ_{i= max(0, j-3)}^{min(12, j+3)} b_k(i)
 *            * a(n) = Σ_{j=0}^{12} b_n(j) * Initial values: a(0)=...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   // for uint64_t, if needed by the build system

seq_t a(idx_t n)
{
    /* Handle the empty string case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* There are 13 digits: 0..12 */
    enum { D = 13 };

    seq_t cur[D];
    seq_t nxt[D];

    /* b_1(j) = 1 for all j */
    for (int j = 0; j < D; ++j) {
        cur[j] = (seq_t)1;
    }

    /* Compute b_k for k = 2 .. n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int j = 0; j < D; ++j) {
            seq_t sum = (seq_t)0;
            int imin = j - 3;
            if (imin < 0) imin = 0;
            int imax = j + 3;
            if (imax > D - 1) imax = D - 1;
            for (int i = imin; i <= imax; ++i) {
                sum += cur[i];
            }
            nxt[j] = sum;
        }
        /* Move nxt into cur for the next iteration */
        for (int j = 0; j < D; ++j) {
            cur[j] = nxt[j];
        }
    }

    /* Sum all b_n(j) to obtain a(n) */
    seq_t total = (seq_t)0;
    for (int j = 0; j < D; ++j) {
        total += cur[j];
    }
    return total;
}
