/*
 * A231620  a(n) = A000930(n^2), where A000930 is Narayana's cows sequence.
 * https://oeis.org/A231620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000930(n * n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = Narayana's cows sequence evaluated at n^2
 * using a 3×3 companion matrix exponentiation.
 * a(0) = 1, a(1) = 1, a(2) = 1, and for k ≥ 3:
 *   a(k) = a(k−1) + a(k−3)      (Narayana's cows recurrence)      */

#include <stdbool.h>

/* Multiply two 3×3 matrices: res = a * b */
static void mat_mul(seq_t a[3][3], seq_t b[3][3], seq_t res[3][3]) {
    seq_t tmp[3][3] = {{0}};
    for (idx_t i = 0; i < 3; ++i)
        for (idx_t k = 0; k < 3; ++k)
            for (idx_t j = 0; j < 3; ++j)
                tmp[i][j] += a[i][k] * b[k][j];
    for (idx_t i = 0; i < 3; ++i)
        for (idx_t j = 0; j < 3; ++j)
            res[i][j] = tmp[i][j];
}

/* Compute res = base^exp  (3×3 matrices) by binary exponentiation */
static void mat_pow(seq_t base[3][3], idx_t exp, seq_t res[3][3]) {
    seq_t identity[3][3] = {{1, 0, 0},
                            {0, 1, 0},
                            {0, 0, 1}};
    for (idx_t i = 0; i < 3; ++i)
        for (idx_t j = 0; j < 3; ++j)
            res[i][j] = identity[i][j];

    seq_t tmp[3][3];
    while (exp > 0) {
        if (exp & 1ULL) {
            mat_mul(res, base, tmp);
            for (idx_t i = 0; i < 3; ++i)
                for (idx_t j = 0; j < 3; ++j)
                    res[i][j] = tmp[i][j];
        }
        mat_mul(base, base, tmp);
        for (idx_t i = 0; i < 3; ++i)
            for (idx_t j = 0; j < 3; ++j)
                base[i][j] = tmp[i][j];
        exp >>= 1ULL;
    }
}

seq_t a(idx_t n) {
    /* Handle the first two squares: n=0,1 -> m=0,1 => a=1    */
    idx_t m = n * n;
    if (m < 3)  /* m = 0 or 1 */
        return 1;

    /* Exponent: m-2 because we start from the vector [a2,a1,a0] */
    idx_t exp = m - 2;

    /* Companion matrix for the recurrence a(k) = a(k-1) + a(k-3)  */
    seq_t M[3][3] = {{1, 0, 1},
                     {1, 0, 0},
                     {0, 1, 0}};

    seq_t P[3][3];
    mat_pow(M, exp, P);

    /* Initial vector [a2,a1,a0] = [1,1,1]  */
    /* Resulting a(m) is the first component of P * [1,1,1]^T */
    seq_t result = P[0][0] + P[0][1] + P[0][2];
    return result;
}
