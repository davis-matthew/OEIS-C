/*
 * A125451  Number of base 16 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1; for n ≥ 1, a(n) = Trace(A^n)
 * where A is the 16 × 16 adjacency matrix of the graph on {0,…,15}
 * with A[i][j] = 1 if |i – j| ≤ 8, otherwise 0.
 * 
 * The implementation pre‑computes powers of A for exponents 2^k and
 * uses binary exponentiation to obtain A^n on demand.
 */

#include <stdbool.h>        /* for bool if needed, not used here */

/* Number of vertices, fixed for this problem */
#define V 16

/* Forward declarations – the build system supplies seq_t and idx_t */
    /* typedef ... seq_t;  typedef ... idx_t;  */

/* Multiply two V×V matrices over seq_t: out = a * b */
static void mat_mul(seq_t out[V][V], const seq_t a[V][V], const seq_t b[V][V]) {
    seq_t temp[V][V];
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < V; ++k) {
                sum += a[i][k] * b[k][j];
            }
            temp[i][j] = sum;
        }
    }
    /* copy back to out */
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            out[i][j] = temp[i][j];
}

/* Base adjacency matrix A[i][j] = 1 if |i-j| ≤ 8 */
static seq_t A[V][V];

/* Pow2[k] = A^(2^k) for k = 0 … 63 (64 bits enough for n ≤ 10^4) */
static seq_t Pow2[64][V][V];
static int cur_pow_bit = -1;  /* -1 means uninitialised */

/* Initialise the adjacency matrix and Pow2[0] */
static void init_pow2(void) {
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j) {
            int diff = i > j ? i - j : j - i;
            A[i][j] = (seq_t)((diff <= 8) ? 1 : 0);
            Pow2[0][i][j] = A[i][j];
        }
    }
    cur_pow_bit = 0;
}

/* Ensure Pow2[0 … bits] are computed */
static void ensure_pow2_upto(int bits) {
    if (cur_pow_bit < 0) init_pow2();
    for (int b = cur_pow_bit + 1; b <= bits; ++b) {
        mat_mul(Pow2[b], Pow2[b - 1], Pow2[b - 1]);  /* square */
    }
    cur_pow_bit = bits;
}

/* Compute a(n) according to the formula above */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;  /* special case */

    /* Find highest set bit of n */
    int bits = 0;
    while ((n >> (bits + 1)) != 0) ++bits;

    ensure_pow2_upto(bits);

    /* res = identity matrix */
    seq_t res[V][V] = {0};
    for (int i = 0; i < V; ++i) res[i][i] = (seq_t)1;

    seq_t temp[V][V];
    for (int bit = 0; bit <= bits; ++bit) {
        if (n & (((idx_t)1) << bit)) {
            mat_mul(temp, res, Pow2[bit]);    /* temp = res * Pow2[bit] */
            /* copy temp into res */
            for (int i = 0; i < V; ++i)
                for (int j = 0; j < V; ++j)
                    res[i][j] = temp[i][j];
        }
    }

    /* Trace of res is the answer */
    seq_t ans = 0;
    for (int i = 0; i < V; ++i) ans += res[i][i];
    return ans;
}
