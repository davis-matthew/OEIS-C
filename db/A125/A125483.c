/*
 * A125483  Number of base 26 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125483
   a(0) = 1
   For n ≥ 1 the value equals the trace of the n‑th power of the 26×26
   adjacency matrix A where A(i,j) = 1 iff |i−j| ≤ 9 (self‑loops included).
   Thus a(n) = trace(Aⁿ). */

#include <stddef.h>

#define N 26                    /* number of digits in base‑26 */

/* Helper: matrix multiplication   dst = a * b   (all NxN) */
static void mat_mul(seq_t dst[N][N], const seq_t a[N][N], const seq_t b[N][N])
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += a[i][k] * b[k][j];
            }
            dst[i][j] = sum;
        }
    }
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 1;               /* special case: empty string */

    /*--- Build adjacency matrix once -------------------------------------*/
    static seq_t adj[N][N];
    static int init_done = 0;
    if (!init_done) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int diff = i > j ? i - j : j - i;
                adj[i][j] = (diff <= 9) ? 1 : 0;
            }
        }
        init_done = 1;
    }

    /*--- Fast exponentiation: compute A^n --------------------------------*/
    /* identity matrix */
    seq_t result[N][N];
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            result[i][j] = (i == j) ? 1 : 0;

    /* current power (starts with A) */
    seq_t power[N][N];
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            power[i][j] = adj[i][j];

    seq_t temp[N][N];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            /* result = result * power */
            mat_mul(temp, result, power);
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    result[i][j] = temp[i][j];
        }
        /* power = power * power */
        mat_mul(temp, power, power);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                power[i][j] = temp[i][j];

        exp >>= 1;
    }

    /*--- Sum the diagonal (trace) ----------------------------------------*/
    seq_t sum = 0;
    for (int i = 0; i < N; ++i)
        sum += result[i][i];

    return sum;
}
