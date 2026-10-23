/*
 * A125419  Number of base 31 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace( M^n ), where M[i][j] = 1 if |i-j| ≤ 6, else 0, for
 *            0 ≤ i,j < 31. The sequence counts length‑n circular strings over
 *            {0,…,30} whose adjacent digits differ by at most 6. a(0)=1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define SIZE 31

/* Multiply two SIZE×SIZE matrices over seq_t. */
static void mat_mul(const seq_t a[SIZE][SIZE],
                    const seq_t b[SIZE][SIZE],
                    seq_t res[SIZE][SIZE])
{
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            seq_t sum = (seq_t)0;
            for (size_t k = 0; k < SIZE; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Initialise the adjacency matrix once */
    static seq_t base[SIZE][SIZE];
    static bool init = false;
    if (!init) {
        init = true;
        for (size_t i = 0; i < SIZE; ++i) {
            for (size_t j = 0; j < SIZE; ++j) {
                size_t diff = (i > j) ? (i - j) : (j - i);
                base[i][j] = (diff <= 6U) ? (seq_t)1 : (seq_t)0;
            }
        }
    }

    /* Binary exponentiation of the matrix */
    seq_t result[SIZE][SIZE];
    seq_t power[SIZE][SIZE];
    seq_t temp[SIZE][SIZE];

    /* Identity matrix */
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* Copy of the base matrix */
    for (size_t i = 0; i < SIZE; ++i) {
        for (size_t j = 0; j < SIZE; ++j) {
            power[i][j] = base[i][j];
        }
    }

    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) {
            mat_mul(result, power, temp);
            for (size_t i = 0; i < SIZE; ++i)
                for (size_t j = 0; j < SIZE; ++j)
                    result[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (exp) {
            mat_mul(power, power, temp);
            for (size_t i = 0; i < SIZE; ++i)
                for (size_t j = 0; j < SIZE; ++j)
                    power[i][j] = temp[i][j];
        }
    }

    /* Trace of the resulting matrix = a(n) */
    seq_t out = (seq_t)0;
    for (size_t i = 0; i < SIZE; ++i)
        out += result[i][i];

    return out;
}
