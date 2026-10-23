/*
 * A109518  a(n)=the (1,2)-entry of the n-th power of the 2 X 2 matrix
 *          [0,1;n-1,3(n-1)].
 * https://oeis.org/A109518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the (1,2)-entry of M^n, where M = [[0,1],[n-1,3(n-1)]]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct { seq_t a00, a01, a10, a11; } matrix;

// 2×2 matrix multiplication: C = A × B
static inline void mat_mul(const matrix *A, const matrix *B, matrix *C)
{
    C->a00 = A->a00 * B->a00 + A->a01 * B->a10;
    C->a01 = A->a00 * B->a01 + A->a01 * B->a11;
    C->a10 = A->a10 * B->a00 + A->a11 * B->a10;
    C->a11 = A->a10 * B->a01 + A->a11 * B->a11;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                               /* safety for offset 1 */
    /* Base matrix for this n */
    matrix base = { 0, 1, (seq_t)(n - 1), (seq_t)3 * (seq_t)(n - 1) };
    matrix result = { 1, 0, 0, 1 };                     /* identity */

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            matrix tmp;
            mat_mul(&result, &base, &tmp);
            result = tmp;
        }
        exp >>= 1;
        if (exp) {                                      /* avoid unnecessary work */
            matrix tmp;
            mat_mul(&base, &base, &tmp);
            base = tmp;
        }
    }
    return result.a01;                                   /* (1,2)-entry */
}
