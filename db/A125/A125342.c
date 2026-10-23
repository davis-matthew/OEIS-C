/*
 * A125342  Number of base 32 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 32×32 adjacency matrix with
 *            A[i][j] = 1 if |i - j| <= 3, else 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n=0, a(0)=1. The function memoizes powers of A
   for all bit positions needed. */

#include <stdint.h>
#include <stdbool.h>

#define V       32          /* number of digits (vertices)      */
#define MAX_BITS 16         /* sufficient for n up to 10 000   */

static seq_t pow_arr[MAX_BITS][V][V];
static bool initialized = false;

/* Multiplication of two V×V matrices: C = A * B */
static void mulmat(const seq_t A[V][V], const seq_t B[V][V], seq_t C[V][V]) {
    seq_t TMP[V][V] = {{0}};
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < V; ++k) {
                sum += A[i][k] * B[k][j];
            }
            TMP[i][j] = sum;
        }
    }
    /* copy result back to C */
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            C[i][j] = TMP[i][j];
}

/* Initialise adjacency matrix and its binary powers */
static void init_matrices(void) {
    /* base matrix A */
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            int diff = i > j ? i - j : j - i;
            pow_arr[0][i][j] = (diff <= 3) ? 1 : 0;
        }
    }
    /* store A^(2^k) for k = 1 … MAX_BITS-1 */
    for (int k = 1; k < MAX_BITS; ++k) {
        mulmat(pow_arr[k - 1], pow_arr[k - 1], pow_arr[k]);
    }
    initialized = true;
}

/* Compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    if (!initialized) {
        init_matrices();
    }

    /* identity matrix as the initial result */
    seq_t res[V][V] = {{0}};
    for (int i = 0; i < V; ++i) {
        res[i][i] = 1;
    }

    /* exponentiation by binary decomposition of n */
    idx_t exp = n;
    int bit = 0;
    while (exp) {
        if (exp & 1ULL) {
            mulmat(res, pow_arr[bit], res);
        }
        exp >>= 1;
        ++bit;
    }

    /* trace of res gives the answer */
    seq_t trace = 0;
    for (int i = 0; i < V; ++i) {
        trace += res[i][i];
    }
    return trace;
}
