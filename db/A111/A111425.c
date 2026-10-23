/*
 * A111425  a(n) = tribonacci(Fibonacci(n)).
 * https://oeis.org/A111425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tribonacci(Fibonacci(n))
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 9, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_fast.h"

typedef struct mat3 { seq_t a[3][3]; } mat3;

static void mat_set_identity(mat3 *m) {
    int i, j;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            m->a[i][j] = (i == j) ? 1 : 0;
}

static void mat_copy(mat3 *dest, const mat3 *src) {
    int i, j;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j)
            dest->a[i][j] = src->a[i][j];
}

static void mat_mul(const mat3 *A, const mat3 *B, mat3 *C) {
    int i, j, k;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            seq_t sum = 0;
            for (k = 0; k < 3; ++k) {
                sum += A->a[i][k] * B->a[k][j];
            }
            C->a[i][j] = sum;
        }
    }
}

static void mat_pow(seq_t exp, mat3 *result) {
    mat3 res;
    mat_set_identity(&res);

    mat3 base;
    base.a[0][0] = 1; base.a[0][1] = 1; base.a[0][2] = 1;
    base.a[1][0] = 1; base.a[1][1] = 0; base.a[1][2] = 0;
    base.a[2][0] = 0; base.a[2][1] = 1; base.a[2][2] = 0;

    mat3 temp;
    seq_t e = exp;
    while (e > 0) {
        if (e % 2 != 0) {
            mat_mul(&res, &base, &temp);
            mat_copy(&res, &temp);
        }
        mat_mul(&base, &base, &temp);
        mat_copy(&base, &temp);
        e = e / 2;
    }
    mat_copy(result, &res);
}

seq_t a(idx_t n) {
    seq_t fib_n;
    seq_t fib_next;
    fib_fast(n, &fib_n, &fib_next);

    if (fib_n <= 1) return 0;
    if (fib_n == 2) return 1;

    seq_t e = fib_n - 2;
    mat3 power;
    mat_pow(e, &power);
    return power.a[0][0];
}
