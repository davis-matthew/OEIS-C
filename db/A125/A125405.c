/*
 * A125405  Number of base 17 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 iff |i-j| ≤ 6, 0 otherwise.
 *            a(0) = 1 is defined separately
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static void mat_mul(int sz, const seq_t A[17][17], const seq_t B[17][17], seq_t C[17][17]) {
    seq_t tmp[17][17];
    const seq_t zero = (seq_t)0;
    for (int i = 0; i < sz; ++i)
        for (int j = 0; j < sz; ++j)
            tmp[i][j] = zero;

    for (int i = 0; i < sz; ++i) {
        for (int k = 0; k < sz; ++k) {
            if (A[i][k] == (seq_t)0) continue;   // skip zero entries
            const seq_t aik = A[i][k];
            for (int j = 0; j < sz; ++j) {
                tmp[i][j] += aik * B[k][j];
            }
        }
    }
    for (int i = 0; i < sz; ++i)
        for (int j = 0; j < sz; ++j)
            C[i][j] = tmp[i][j];
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;  // offset 0 term

    /* Build adjacency matrix once */
    static seq_t adj[17][17];
    static bool init = false;
    if (!init) {
        for (int i = 0; i < 17; ++i)
            for (int j = 0; j < 17; ++j) {
                int d = i > j ? i - j : j - i;
                adj[i][j] = (d <= 6) ? (seq_t)1 : (seq_t)0;
            }
        init = true;
    }

    /* Binary exponentiation of adjacency matrix */
    seq_t result[17][17];
    seq_t power[17][17];
    const seq_t zero = (seq_t)0;
    const seq_t one  = (seq_t)1;
    // identity matrix into result
    for (int i = 0; i < 17; ++i)
        for (int j = 0; j < 17; ++j)
            result[i][j] = (i == j) ? one : zero;
    // copy adjacency into power
    for (int i = 0; i < 17; ++i)
        for (int j = 0; j < 17; ++j)
            power[i][j] = adj[i][j];

    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1)
            mat_mul(17, result, power, result);
        mat_mul(17, power, power, power);
        exp >>= 1;
    }

    /* Trace of result matrix is the desired count */
    seq_t ans = (seq_t)0;
    for (int i = 0; i < 17; ++i)
        ans += result[i][i];
    return ans;
}
