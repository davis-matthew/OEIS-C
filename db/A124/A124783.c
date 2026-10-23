/*
 * A124783  Number of base 28 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{j=1..28} (1 + 2*cos(j*Pi/29))^n (number of smooth
 *            cyclic words of length n over {1..28})
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define K 28

/* matrix multiplication: dst = a * b  (all KxK) */
static void mat_mul(const seq_t a[K][K], const seq_t b[K][K], seq_t dst[K][K]) {
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < K; ++k) {
                sum += a[i][k] * b[k][j];
            }
            dst[i][j] = sum;
        }
    }
}

/* a(n): number of base 28 circular n‑digit numbers with adjacent digits differing by ≤1 */
seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* adjacency matrix of the path graph with self loops */
    seq_t base[K][K];
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) base[i][j] = 0;
        base[i][i] = 1;
        if (i > 0)     base[i][i - 1] = 1;
        if (i + 1 < K) base[i][i + 1] = 1;
    }

    /* result = I, power = base */
    seq_t result[K][K], power[K][K];
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            result[i][j] = (i == j) ? 1 : 0;
            power[i][j]  = base[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            seq_t temp[K][K];
            mat_mul(result, power, temp);
            for (int i = 0; i < K; ++i)
                for (int j = 0; j < K; ++j)
                    result[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (exp == 0) break;
        seq_t temp2[K][K];
        mat_mul(power, power, temp2);
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                power[i][j] = temp2[i][j];
    }

    seq_t sum = 0;
    for (int i = 0; i < K; ++i) sum += result[i][i];
    return sum;
}
