/*
 * A125368  Number of base 31 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A125368: Number of base‑31 circular n‑digit numbers 
   with adjacent digits differing by at most 4.
   Formula implemented: a(0)=1, for n≥1  a(n)=trace(A^n)
   where A is the 31×31 adjacency matrix with A[i][j]=1 if |i−j|≤4
   (digits 0..30) and 0 otherwise.
*/
#include <stdint.h>

/* --------------------------------------------------------------------- */
/* Lazy build of the adjacency matrix. The matrix is the same for all   */
/* invocations, so we compute it only once.                              */
/* --------------------------------------------------------------------- */
#define B 31
static seq_t     adj[B][B];
static unsigned int adj_built = 0;

static void build_adj (void)
{
    if (adj_built) return;
    for (int i = 0; i < B; ++i)
        for (int j = 0; j < B; ++j)
            adj[i][j] = ( (i > j) ? (i - j) : (j - i) ) <= 4 ? 1 : 0;
    adj_built = 1;
}

/* --------------------------------------------------------------------- */
/* Matrix multiplication:  C = A * B   (dimensions 31×31).               */
/* Uses a temporary buffer to avoid overwriting source matrices.         */
/* --------------------------------------------------------------------- */
static void mat_mult (seq_t dst[B][B], const seq_t a[B][B], const seq_t b[B][B])
{
    seq_t tmp[B][B] = {{0}};
    for (int i = 0; i < B; ++i) {
        for (int k = 0; k < B; ++k) {
            if (a[i][k] == 0) continue;
            seq_t aik = a[i][k];
            for (int j = 0; j < B; ++j) {
                tmp[i][j] += aik * b[k][j];
            }
        }
    }
    /* Copy result back to dst */
    for (int i = 0; i < B; ++i)
        for (int j = 0; j < B; ++j)
            dst[i][j] = tmp[i][j];
}

/* --------------------------------------------------------------------- */
/* Compute a(n).  For n==0 return 1.  For n≥1 compute trace of A^n.     */
/* Uses fast binary exponentiation (O(log n·B³)).                        */
/* --------------------------------------------------------------------- */
seq_t a (idx_t n)
{
    if (n == 0) return 1;    /* base case as defined in OEIS */

    build_adj ();           /* ensure adjacency matrix built */

    /* identity matrix */
    seq_t result[B][B];
    for (int i = 0; i < B; ++i)
        for (int j = 0; j < B; ++j)
            result[i][j] = (i == j) ? 1 : 0;

    /* base matrix = adjacency matrix */
    seq_t base[B][B];
    for (int i = 0; i < B; ++i)
        for (int j = 0; j < B; ++j)
            base[i][j] = adj[i][j];

    /* binary exponentiation */
    while (n > 0) {
        if (n & 1) {
            mat_mult(result, result, base);
        }
        mat_mult(base, base, base);
        n >>= 1;
    }

    /* trace of result */
    seq_t trace = 0;
    for (int i = 0; i < B; ++i)
        trace += result[i][i];

    return trace;
}
