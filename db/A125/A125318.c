/*
 * A125318  Number of base 8 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = trace(A^n) for n ≥ 1,
 *   where A is the 8×8 adjacency matrix of the graph on vertices 0…7
 *   with edges between i and j iff |i – j| ≤ 3.  For n = 0 we define a(0) = 1.
 *   The function implements this definition efficiently with binary
 *   exponentiation and memoises the powers of A for repeated calls.
 */

#include <stdint.h>

typedef struct {
    seq_t a[8][8];
} mat8;

/* Multiply two 8×8 matrices: r = x * y */
static void mat_mul(const mat8 *x, const mat8 *y, mat8 *r)
{
    mat8 temp = {0};
    for (int i = 0; i < 8; ++i) {
        for (int k = 0; k < 8; ++k) {
            seq_t aik = x->a[i][k];
            if (aik == 0) continue;
            for (int j = 0; j < 8; ++j) {
                temp.a[i][j] = temp.a[i][j] + aik * y->a[k][j];
            }
        }
    }
    *r = temp;
}

/* Compute base^exp using exponentiation by squaring: res = base^exp */
static void mat_pow(const mat8 *base, idx_t exp, mat8 *res)
{
    mat8 result = {0};
    mat8 power = *base;

    /* Identity matrix */
    for (int i = 0; i < 8; ++i) result.a[i][i] = 1;

    while (exp != 0) {
        if (exp & 1) {
            mat8 tmp;
            mat_mul(&result, &power, &tmp);
            result = tmp;
        }
        exp >>= 1;
        if (exp) {
            mat8 tmp;
            mat_mul(&power, &power, &tmp);
            power = tmp;
        }
    }
    *res = result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    /* Build the adjacency matrix A */
    mat8 A = {0};
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            int diff = (i > j) ? (i - j) : (j - i);
            if (diff <= 3) {
                A.a[i][j] = 1;
            } else {
                A.a[i][j] = 0;
            }
        }
    }

    /* Compute A^n */
    mat8 An;
    mat_pow(&A, n, &An);

    /* Sum diagonal entries for trace */
    seq_t sum = 0;
    for (int i = 0; i < 8; ++i) {
        sum = sum + An.a[i][i];
    }
    return sum;
}
