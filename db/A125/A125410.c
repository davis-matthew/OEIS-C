/*
 * A125410  Number of base 22 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 22×22 adjacency matrix * with
 *            A[i][j] = 1 iff |i-j|≤6. * a(0) is defined to be 1 (empty
 *            circular number)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Matrix multiplication: res = a * b  (all 22×22). */
static void mat_mul(seq_t res[22][22], const seq_t a[22][22], const seq_t b[22][22])
{
    for (idx_t i = 0; i < 22; ++i) {
        for (idx_t j = 0; j < 22; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 22; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    static bool initialized = false;
    static seq_t base[22][22];

    if (!initialized) {
        for (idx_t i = 0; i < 22; ++i) {
            for (idx_t j = 0; j < 22; ++j) {
                idx_t diff = (i > j) ? i - j : j - i;
                base[i][j] = (diff <= 6) ? 1 : 0;
            }
        }
        initialized = true;
    }

    seq_t result[22][22];
    seq_t power[22][22];
    seq_t temp[22][22];

    /* identity matrix */
    for (idx_t i = 0; i < 22; ++i) {
        for (idx_t j = 0; j < 22; ++j) {
            result[i][j] = (i == j) ? 1 : 0;
            power[i][j] = base[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(temp, result, power);
            for (idx_t i = 0; i < 22; ++i)
                for (idx_t j = 0; j < 22; ++j)
                    result[i][j] = temp[i][j];
        }
        mat_mul(temp, power, power);
        for (idx_t i = 0; i < 22; ++i)
            for (idx_t j = 0; j < 22; ++j)
                power[i][j] = temp[i][j];
        exp >>= 1;
    }

    seq_t total = 0;
    for (idx_t i = 0; i < 22; ++i) {
        total += result[i][i];
    }
    return total;
}
