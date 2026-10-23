/*
 * A125461  Number of base 26 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| <= 8 else 0 for 0
 *            <= i,j < 26 Initial value: a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static void mat_mul(const seq_t a[26][26], const seq_t b[26][26], seq_t res[26][26]) {
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 26; ++k) {
                sum = sum + a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;  // special case

    /* adjacency matrix A */
    static seq_t A[26][26];
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                idx_t d = (i >= j) ? (i - j) : (j - i);
                A[i][j] = (d <= 8) ? 1 : 0;
            }
        }
        initialized = 1;
    }

    seq_t result[26][26];
    seq_t cur[26][26];
    seq_t temp[26][26];

    /* result = identity matrix */
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            result[i][j] = (i == j) ? 1 : 0;
            cur[i][j]   = A[i][j];
        }
    }

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            mat_mul(result, cur, temp);
            for (int i = 0; i < 26; ++i)
                for (int j = 0; j < 26; ++j)
                    result[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (exp) {
            mat_mul(cur, cur, temp);
            for (int i = 0; i < 26; ++i)
                for (int j = 0; j < 26; ++j)
                    cur[i][j] = temp[i][j];
        }
    }

    seq_t sum = 0;
    for (int i = 0; i < 26; ++i) {
        sum = sum + result[i][i];
    }
    return sum;
}
