/*
 * A124717  Number of base 24 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124717
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 24×24 adjacency matrix of a
 *            path of length 24 with a loop on every vertex
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1;  a(1) = 24;  a(2) = 70;  a(3) = 162;  ... */

#include <stddef.h>               /* for size_t */

#define M 24

typedef struct {
    seq_t m[M][M];
} matrix;

/* Multiply two M×M matrices.  The result fits in seq_t because
   each entry of the adjacency matrix is 0 or 1, and the product
   of counts of walks never exceeds the number of closed walks,
   which is stored in seq_t.                           */
static matrix mul(const matrix *a, const matrix *b)
{
    matrix c = {0};
    for (size_t i = 0; i < M; ++i)
        for (size_t k = 0; k < M; ++k) {
            seq_t aik = a->m[i][k];
            if (!aik) continue;
            for (size_t j = 0; j < M; ++j)
                c.m[i][j] += aik * b->m[k][j];
        }
    return c;
}

/* Return the M×M identity matrix. */
static matrix identity(void)
{
    matrix I = {0};
    for (size_t i = 0; i < M; ++i) I.m[i][i] = 1;
    return I;
}

/* Build the adjacency matrix for the graph.  Vertex i (0‑based)
   is connected to i itself, i-1 and i+1 (when they exist). */
static matrix base_matrix(void)
{
    matrix A = {0};
    for (size_t i = 0; i < M; ++i) {
        A.m[i][i] = 1;                 /* loop */
        if (i + 1 < M) A.m[i][i + 1] = 1;   /* edge to i+1 */
        if (i > 0)       A.m[i][i - 1] = 1;   /* edge to i-1 */
    }
    return A;
}

/* Number of length‑n closed walks on the 24‑vertex path with loops. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* A^0 = I, trace = 24?  but by definition a(0)=1 */

    matrix result = identity();
    matrix power  = base_matrix();

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) result = mul(&result, &power);
        power = mul(&power, &power);
        exp >>= 1;
    }

    seq_t sum = 0;
    for (size_t i = 0; i < M; ++i)
        sum += result.m[i][i];
    return sum;
}
