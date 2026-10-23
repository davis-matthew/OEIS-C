/*
 * A125442  Number of base 30 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1, a(n) = trace(A^n) for n>0, where A[i][j] = 1 if |i-j|
 *            ≤ 7 (base 30 digits are 0…29)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The OEIS definition: count base‑30 circular n‑digit numbers where
   adjacent digits differ by at most 7.  The number is the trace of the
   nth power of the adjacency matrix A of the 30‑digit directed graph.  */

#include <stdbool.h>

/* constant values that must be compile‑time constants for C */
#define BASE_DIGITS 30   /* 30 digits in base 30 */
#define MAX_P       15   /* 2^14 = 16384 > 10000, enough for binary exp. */

/* package-supplied types */
 /* idx_t, seq_t are defined by the build system. */

/* pre‑computed powers of the adjacency matrix: powA[p] = A^(2^p)   */
static seq_t powA[MAX_P][BASE_DIGITS][BASE_DIGITS];
static bool initialized = false;

/* matrix multiplication: C = A · B (size BASE_DIGITS×BASE_DIGITS)  */
static void matmul(const seq_t A[BASE_DIGITS][BASE_DIGITS],
                   const seq_t B[BASE_DIGITS][BASE_DIGITS],
                   seq_t C[BASE_DIGITS][BASE_DIGITS])
{
    idx_t i, j, k;
    for (i = 0; i < BASE_DIGITS; ++i) {
        for (j = 0; j < BASE_DIGITS; ++j) {
            seq_t sum = 0;
            for (k = 0; k < BASE_DIGITS; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* build adjacency matrix A and its binary powers            */
static void init_powers(void)
{
    if (initialized)
        return;
    initialized = true;

    seq_t A[BASE_DIGITS][BASE_DIGITS];
    idx_t i, j;

    /* adjacency matrix: A[i][j] = 1 if |i-j| <= 7, else 0         */
    for (i = 0; i < BASE_DIGITS; ++i) {
        for (j = 0; j < BASE_DIGITS; ++j) {
            idx_t d = (i > j) ? (i - j) : (j - i);
            A[i][j] = (d <= 7) ? 1 : 0;
        }
    }

    /* powA[0] = A */
    for (i = 0; i < BASE_DIGITS; ++i)
        for (j = 0; j < BASE_DIGITS; ++j)
            powA[0][i][j] = A[i][j];

    /* compute powA[p] = powA[p-1] · powA[p-1] for p>=1             */
    for (idx_t p = 1; p < MAX_P; ++p) {
        matmul(powA[p-1], powA[p-1], powA[p]);
    }
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* The empty number is counted as one way.                 */
    if (n == 0)
        return (seq_t)1;

    init_powers();

    /* result matrix starts as the identity matrix.             */
    seq_t res[BASE_DIGITS][BASE_DIGITS];
    idx_t i, j;
    for (i = 0; i < BASE_DIGITS; ++i)
        for (j = 0; j < BASE_DIGITS; ++j)
            res[i][j] = (i == j) ? 1 : 0;

    /* binary exponentiation of the matrix using prepared powers. */
    idx_t m = n;
    idx_t bit = 0;
    while (m != 0) {
        if (m & 1ULL) {
            seq_t tmp[BASE_DIGITS][BASE_DIGITS];
            matmul(res, powA[bit], tmp);
            for (i = 0; i < BASE_DIGITS; ++i)
                for (j = 0; j < BASE_DIGITS; ++j)
                    res[i][j] = tmp[i][j];
        }
        m >>= 1;
        ++bit;
    }

    /* trace of A^n gives the desired count.                      */
    seq_t trace = 0;
    for (i = 0; i < BASE_DIGITS; ++i)
        trace += res[i][i];

    return trace;
}
