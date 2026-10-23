/*
 * A125341  Number of base 31 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 for n = 0, otherwise a(n) = Trace(A^n) where A is the
 *            31 × 31 adjacency matrix with A[i][j] = 1 if |i - j| ≤ 3
 *            (self‑loops included)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values (for reference):
   a(0)=1, a(1)=31, a(2)=205, a(3)=1063, a(4)=6469.                                    */
#include <stdbool.h>

#define D 31  /* number of digits in base 31 */

static bool initialized = false;
static seq_t adj[D * D];

/* multiply two D×D matrices: C = A * B  */
static void mat_mul(const seq_t *A, const seq_t *B, seq_t *C)
{
    for (idx_t i = 0; i < D; ++i) {
        for (idx_t j = 0; j < D; ++j) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < D; ++k) {
                sum = sum + (A[i * D + k] * B[k * D + j]);
            }
            C[i * D + j] = sum;
        }
    }
}

/* build the adjacency matrix once */
static void init_adj(void)
{
    if (initialized) return;
    for (idx_t i = 0; i < D; ++i) {
        for (idx_t j = 0; j < D; ++j) {
            idx_t diff = (i > j) ? (i - j) : (j - i);
            if (diff <= 3) {
                adj[i * D + j] = (seq_t)1;
            } else {
                adj[i * D + j] = (seq_t)0;
            }
        }
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* definition for a(0) */

    init_adj();

    /* temporary matrices for exponentiation */
    seq_t result[D * D];
    seq_t base[D * D];
    seq_t temp[D * D];

    /* result = identity matrix, base = adjacency matrix */
    for (idx_t i = 0; i < D * D; ++i) {
        result[i] = (seq_t)0;
        base[i]   = adj[i];
    }
    for (idx_t i = 0; i < D; ++i) {
        result[i * D + i] = (seq_t)1;
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            mat_mul(result, base, temp);
            for (idx_t i = 0; i < D * D; ++i) result[i] = temp[i];
        }
        mat_mul(base, base, temp);
        for (idx_t i = 0; i < D * D; ++i) base[i] = temp[i];
        exp >>= 1;
    }

    /* trace of result is the number of closed walks of length n */
    seq_t trace = (seq_t)0;
    for (idx_t i = 0; i < D; ++i) {
        trace = trace + result[i * D + i];
    }
    return trace;
}
