/*
 * A125471  Number of base 14 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 for n = 0 * a(n) = trace(A^n) for n >= 1 * where A is
 *            the 14×14 binary adjacency matrix * with A[i][j] = 1 if |i-j| <=
 *            9 (else 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the big‑integer multiplication helper provided by the
 * build environment.  It guarantees correct behaviour on both
 * fixed‑size and arbitrary‑precision seq_t types. */
#include "../utils/seq_mul.h"

/* ------------------------------------------------------------------ */
/* Helper definitions                                               */
/* ------------------------------------------------------------------ */
#define DIM 14

/* copy matrix src into dest (both are DIM×DIM) */
static void mat_copy(seq_t dest[DIM][DIM], const seq_t src[DIM][DIM])
{
    for (idx_t i = 0; i < DIM; ++i)
        for (idx_t j = 0; j < DIM; ++j)
            dest[i][j] = src[i][j];
}

/* dst = A * B         (both DIM×DIM) */
static void mat_mul(seq_t dst[DIM][DIM],
                    const seq_t A[DIM][DIM],
                    const seq_t B[DIM][DIM])
{
    for (idx_t i = 0; i < DIM; ++i) {
        for (idx_t j = 0; j < DIM; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < DIM; ++k) {
                seq_t prod = seq_mul(A[i][k], B[k][j]);
                sum += prod;
            }
            dst[i][j] = sum;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Main function                                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Base case: empty circular number */
    if (n == 0)
        return (seq_t)1;

    /* Build the adjacency matrix A */
    seq_t A[DIM][DIM];
    for (idx_t i = 0; i < DIM; ++i) {
        for (idx_t j = 0; j < DIM; ++j) {
            idx_t diff = (i > j) ? i - j : j - i;
            A[i][j] = (diff <= 9) ? 1 : 0;
        }
    }

    /* Exponentiation by squaring: result = I, base = A */
    seq_t result[DIM][DIM];
    seq_t base[DIM][DIM];
    seq_t tmp[DIM][DIM];

    /* identity matrix */
    for (idx_t i = 0; i < DIM; ++i) {
        for (idx_t j = 0; j < DIM; ++j)
            result[i][j] = (i == j) ? 1 : 0;
    }
    /* base = A */
    mat_copy(base, A);

    /* exponentiation loop */
    while (n > 0) {
        if (n & 1) {
            mat_mul(tmp, result, base);
            mat_copy(result, tmp);
        }
        mat_mul(tmp, base, base);
        mat_copy(base, tmp);
        n >>= 1;
    }

    /* trace of result gives a(n) for n >= 1 */
    seq_t ans = 0;
    for (idx_t i = 0; i < DIM; ++i)
        ans += result[i][i];

    return ans;
}
