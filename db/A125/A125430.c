/*
 * A125430  Number of base 18 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n >= 1, where A[i][j] = 1 if |i-j| <= 7
 *            else 0. a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define S 18          /* matrix side */

/* multiply two SxS matrices over seq_t */
static void mat_mul(seq_t res[S][S], const seq_t a[S][S], const seq_t b[S][S])
{
    seq_t tmp[S][S];
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < S; ++k)
                sum += a[i][k] * b[k][j];
            tmp[i][j] = sum;
        }
    }
    for (int i = 0; i < S; ++i)
        for (int j = 0; j < S; ++j)
            res[i][j] = tmp[i][j];
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;                     /* base case */

    /* precompute powers of the adjacency matrix A: A^(2^k) */
    static seq_t pow2[15][S][S];
    static int initialized = 0;

    if (!initialized) {
        seq_t A[S][S];
        for (int i = 0; i < S; ++i) {
            for (int j = 0; j < S; ++j) {
                int d = (i < j) ? (j - i) : (i - j);
                A[i][j] = (d <= 7) ? 1 : 0;
            }
        }
        /* pow2[0] = A */
        for (int i = 0; i < S; ++i)
            for (int j = 0; j < S; ++j)
                pow2[0][i][j] = A[i][j];

        /* subsequent powers: pow2[k] = pow2[k-1]^2 */
        for (int k = 1; k < 15; ++k)
            mat_mul(pow2[k], pow2[k - 1], pow2[k - 1]);

        initialized = 1;
    }

    /* result matrix initialized to identity */
    seq_t R[S][S];
    for (int i = 0; i < S; ++i)
        for (int j = 0; j < S; ++j)
            R[i][j] = (i == j) ? 1 : 0;

    /* binary exponentiation using precomputed powers */
    idx_t mask = 1;
    for (int bit = 0; bit < 15; ++bit) {
        if (n & mask) {
            seq_t temp[S][S];
            mat_mul(temp, R, pow2[bit]);
            for (int i = 0; i < S; ++i)
                for (int j = 0; j < S; ++j)
                    R[i][j] = temp[i][j];
        }
        mask <<= 1;
    }

    /* trace of the resulting matrix */
    seq_t result = 0;
    for (int i = 0; i < S; ++i)
        result += R[i][i];

    return result;
}
