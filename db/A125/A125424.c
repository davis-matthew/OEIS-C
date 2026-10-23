/*
 * A125424  Number of base 12 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(Aᵗⁿ) where Aᵢⱼ = 1 if |i−j| ≤ 7 (0‑based indices,
 *            0…11)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function counts circular base‑12 n‑digit numbers whose adjacent
   digits differ by at most 7. It computes the trace of the n‑th power
   of a 12×12 adjacency matrix.  The implementation uses standard
   matrix exponentiation with O(12³ log n) operations, which is
   efficient for all requested n (up to ~10000). */

#include <stdbool.h>

/* constant number of vertices (digits 0…11) */
enum { N = 12 };

/* multiply two N×N matrices: res = a * b */
static void mat_mul(const seq_t a[N][N], const seq_t b[N][N], seq_t res[N][N])
{
    int i, j, k;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            seq_t sum = (seq_t)0;
            for (k = 0; k < N; ++k)
                sum += a[i][k] * b[k][j];
            res[i][j] = sum;
        }
    }
}

/* fill the adjacency matrix once */
static void build_adjacency(seq_t adj[N][N])
{
    int i, j;
    for (i = 0; i < N; ++i) {
        for (j = 0; j < N; ++j) {
            int diff = i - j;
            if (diff < 0)
                diff = -diff;
            adj[i][j] = (diff <= 7) ? (seq_t)1 : (seq_t)0;
        }
    }
}

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* adjacency matrix, built only once */
    static seq_t Ad[N][N];
    static bool Ad_init = false;
    if (!Ad_init) {
        build_adjacency(Ad);
        Ad_init = true;
    }

    /* matrices for exponentiation */
    seq_t pow[N][N], cur[N][N], temp[N][N];

    /* initialize pow as identity matrix */
    int i, j;
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            pow[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* copy adjacency matrix into cur */
    for (i = 0; i < N; ++i)
        for (j = 0; j < N; ++j)
            cur[i][j] = Ad[i][j];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(pow, cur, temp);
            for (i = 0; i < N; ++i)
                for (j = 0; j < N; ++j)
                    pow[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (exp) {
            mat_mul(cur, cur, temp);
            for (i = 0; i < N; ++i)
                for (j = 0; j < N; ++j)
                    cur[i][j] = temp[i][j];
        }
    }

    /* trace of pow is the desired count */
    seq_t trace = (seq_t)0;
    for (i = 0; i < N; ++i)
        trace += pow[i][i];

    return trace;
}
