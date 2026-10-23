/*
 * A125328  Number of base 18 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125328
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(M^n) for n >= 1, a(0) = 1 where M is the 18×18
 *            adjacency matrix: M[i][j] = 1 if |i-j| <= 3, 0 otherwise
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Private helper: multiply two 18×18 matrices over seq_t. */
static void mul_matrix(const seq_t A[18][18], const seq_t B[18][18], seq_t C[18][18]) {
    for (size_t i = 0; i < 18; ++i) {
        for (size_t j = 0; j < 18; ++j) {
            seq_t sum = 0;
            for (size_t k = 0; k < 18; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* empty number */
    /* static adjacency matrix, initialized once */
    static seq_t M[18][18];
    static bool init = false;
    if (!init) {
        for (size_t i = 0; i < 18; ++i) {
            for (size_t j = 0; j < 18; ++j) {
                if ( (i > j ? i - j : j - i) <= (idx_t)3 ) {
                    M[i][j] = (seq_t)1;
                } else {
                    M[i][j] = (seq_t)0;
                }
            }
        }
        init = true;
    }

    /* Fast exponentiation of M^n, using 18×18 matrices */
    seq_t result[18][18];
    seq_t base[18][18];
    seq_t tmp[18][18];

    /* result = Identity */
    for (size_t i = 0; i < 18; ++i) {
        for (size_t j = 0; j < 18; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            base[i][j] = M[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mul_matrix(result, base, tmp);
            for (size_t i = 0; i < 18; ++i)
                for (size_t j = 0; j < 18; ++j)
                    result[i][j] = tmp[i][j];
        }
        mul_matrix(base, base, tmp);
        for (size_t i = 0; i < 18; ++i)
            for (size_t j = 0; j < 18; ++j)
                base[i][j] = tmp[i][j];
        exp >>= 1;
    }

    /* compute the trace */
    seq_t trace = 0;
    for (size_t i = 0; i < 18; ++i)
        trace += result[i][i];

    return trace;
}
