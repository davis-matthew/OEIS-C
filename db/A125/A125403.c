/*
 * A125403  Number of base 15 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(M^n) for n≥1, with a(0)=1, where M is the 15×15
 *            adjacency matrix of the graph on vertices {0,…,14} with edges
 *            (i,j) if |i–j|≤6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Allow use of bool if needed */
#include <stdbool.h>

/* Multiply two 15×15 matrices over the algebra of seq_t. */
static void mat_mul(const seq_t A[15][15], const seq_t B[15][15], seq_t R[15][15])
{
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < 15; ++k)
                sum += A[i][k] * B[k][j];
            R[i][j] = sum;
        }
    }
}

/* Public function computing a(n) for the OEIS sequence A125403. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;                    /* a(0) = 1 by definition */

    /* Build the adjacency matrix once. */
    static int built = 0;
    static seq_t M[15][15];
    if (!built) {
        built = 1;
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                int diff = i - j;
                if (diff < 0) diff = -diff;
                M[i][j] = (diff <= 6) ? (seq_t)1 : (seq_t)0;
            }
        }
    }

    /* result = identity matrix */
    seq_t result[15][15] = {0};
    for (int i = 0; i < 15; ++i)
        result[i][i] = (seq_t)1;

    /* power = M */
    seq_t power[15][15];
    for (int i = 0; i < 15; ++i)
        for (int j = 0; j < 15; ++j)
            power[i][j] = M[i][j];

    /* Fast exponentiation of matrix M to the nth power. */
    idx_t e = n;
    seq_t temp[15][15];
    while (e) {
        if (e & (idx_t)1) {
            mat_mul(result, power, temp);
            for (int i = 0; i < 15; ++i)
                for (int j = 0; j < 15; ++j)
                    result[i][j] = temp[i][j];
        }
        mat_mul(power, power, temp);
        for (int i = 0; i < 15; ++i)
            for (int j = 0; j < 15; ++j)
                power[i][j] = temp[i][j];
        e >>= 1;
    }

    /* Sum the diagonal of M^n to obtain a(n). */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < 15; ++i)
        sum += result[i][i];
    return sum;
}
