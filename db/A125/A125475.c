/*
 * A125475  Number of base 18 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1, a(0) = 1 where A is the 18×18
 *            adjacency matrix defined by A[i][j] = 1 if |i - j| ≤ 9,
 *            otherwise 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define N 18
#define MAX_BITS 64

static seq_t P[MAX_BITS][N][N];
static int computed_bits = 0;   /* number of powers of A stored */

/* multiply two N×N matrices over seq_t */
static void matmul(const seq_t a[N][N], const seq_t b[N][N], seq_t res[N][N]) {
    for (idx_t i = 0; i < N; ++i) {
        for (idx_t j = 0; j < N; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < N; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

/* initialise the base adjacency matrix A in P[0] */ 
static void init_powers(void) {
    if (computed_bits != 0) return;      /* already initialised */
    for (idx_t i = 0; i < N; ++i) {
        for (idx_t j = 0; j < N; ++j) {
            seq_t val = ( (i >= j ? i - j : j - i) <= 9 ? 1 : 0 );
            P[0][i][j] = val;
        }
    }
    computed_bits = 1;   /* P[0] ready, next free index is 1 */
}

/* return a(n) as defined in the problem */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;      /* definition of a(0) */

    init_powers();                    /* ensure P[0] exists */

    /* determine highest bit needed */
    idx_t max_bit = 0;
    while (((idx_t)1 << (max_bit + 1)) <= n) ++max_bit;

    /* compute powers of A up to max_bit if not already cached */
    while (computed_bits <= max_bit) {
        matmul(P[computed_bits - 1], P[computed_bits - 1], P[computed_bits]);
        ++computed_bits;
    }

    /* identity matrix for exponentiation */
    seq_t res[N][N];
    for (idx_t i = 0; i < N; ++i) {
        for (idx_t j = 0; j < N; ++j) {
            res[i][j] = (i == j) ? 1 : 0;
        }
    }

    seq_t tmp[N][N];
    idx_t k = 0;
    idx_t v = n;
    while (v != 0) {
        if (v & 1) {
            matmul(res, P[k], tmp);
            /* copy tmp → res */
            for (idx_t i = 0; i < N; ++i)
                for (idx_t j = 0; j < N; ++j)
                    res[i][j] = tmp[i][j];
        }
        v >>= 1;
        ++k;
    }

    /* trace of the resulting matrix */
    seq_t sum = 0;
    for (idx_t i = 0; i < N; ++i) sum += res[i][i];
    return sum;
}
