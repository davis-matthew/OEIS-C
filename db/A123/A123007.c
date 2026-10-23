/*
 * A123007  Expansion of x*(1+x)/(1 -2*x -9*x^2).
 * https://oeis.org/A123007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 2*a(n-1) + 9*a(n-2) for n > 2 * a(1) = 1, a(2) = 3 * *
 *            Uses fast exponentiation of the recurrence matrix [[2,9],[1,0]]
 *            * to obtain a(n) in O(log n) operations. No dynamic memory is *
 *            allocated, so the implementation works with both plain integer *
 *            typedefs and arbitrary‑precision integer...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef struct {
    seq_t m00, m01;
    seq_t m10, m11;
} matrix2x2;

/* matrix multiplication: C = A * B */
static matrix2x2 mat_mul(matrix2x2 A, matrix2x2 B)
{
    matrix2x2 C;
    C.m00 = A.m00 * B.m00 + A.m01 * B.m10;
    C.m01 = A.m00 * B.m01 + A.m01 * B.m11;
    C.m10 = A.m10 * B.m00 + A.m11 * B.m10;
    C.m11 = A.m10 * B.m01 + A.m11 * B.m11;
    return C;
}

/* matrix exponentiation: returns M^exp */
static matrix2x2 mat_pow(matrix2x2 M, idx_t exp)
{
    matrix2x2 result = { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1 }; /* identity */
    while (exp > 0) {
        if (exp & 1) {
            result = mat_mul(result, M);
        }
        M = mat_mul(M, M);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* unused, but defined */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;

    /* recurrence matrix M = [[2,9],[1,0]] */
    matrix2x2 M = { (seq_t)2, (seq_t)9, (seq_t)1, (seq_t)0 };

    /* M^(n-2) */
    matrix2x2 P = mat_pow(M, n - 2);

    seq_t a2 = (seq_t)3;
    seq_t a1 = (seq_t)1;

    /* a(n) = P.m00 * a2 + P.m01 * a1 */
    return P.m00 * a2 + P.m01 * a1;
}
