/*
 * A233639  Number of (n+1)X(3+1) 0..2 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 2.
 * https://oeis.org/A233639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A233639 – counting (n+1)×4 arrays with the
 *  2×2 subblock edge‑difference‑squared sum equal to 2.
 *
 * The sequence is obtained by building a 3‑ary directed graph
 * whose vertices are all 3^4=81 possible row patterns.
 * A directed edge r → s exists iff for each of the 3
 * consecutive pairs of columns the 2×2 block formed by r and s
 * satisfies (|a−b|²+|c−d|²+|a−c|²+|b−d|²==2).
 *
 * For a given n we need the number of walks of length n in this
 * graph (each walk corresponds to a (n+1)×4 array) starting from
 * any vertex.  This is obtained by computing
 *
 *      v₀ = (1,1,…,1)                     // length‑0 walks
 *      vₙ = v₀ · Mⁿ
 *
 * where M is the 81×81 adjacency matrix (entries 0/1).
 * The answer is Σₖ vₙ[k].  We evaluate Mⁿ·v₀ by binary exponentiation.
 * The algorithm runs in O(81³ log n) time and uses only addition
 * and multiplication of `seq_t`, so it works with 8, 16, 32, 64,
 * 128‑bit or arbitrary‑precision integers.
 */

#include <stdlib.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* the sequence is 1‑based */

    /* Build the adjacency matrix once.  M[i][j] = 1 if a walk
     * from row i to row j is allowed, otherwise 0.           */
    static seq_t M[81][81];
    static int built = 0;
    if (!built) {
        int row[4], col[4];
        for (int r = 0; r < 81; ++r) {
            int x = r;
            for (int k = 3; k >= 0; --k) { row[k] = x % 3; x /= 3; }
            for (int s = 0; s < 81; ++s) {
                int y = s;
                for (int k = 3; k >= 0; --k) { col[k] = y % 3; y /= 3; }
                int ok = 1;
                for (int j = 0; j < 3 && ok; ++j) {
                    int a = row[j], b = row[j+1];
                    int c = col[j], d = col[j+1];
                    int da = a > b ? a - b : b - a;
                    int db = c > d ? c - d : d - c;
                    int dc = a > c ? a - c : c - a;
                    int dd = b > d ? b - d : d - b;
                    int sum = da*da + db*db + dc*dc + dd*dd;
                    if (sum != 2) ok = 0;
                }
                M[r][s] = ok ? (seq_t)1 : (seq_t)0;
            }
        }
        built = 1;
    }

    /* Temporary matrices for exponentiation.  newM holds the
     * result of a multiplication, Mpow holds the current power. */
    seq_t Mpow[81][81];         /* current matrix power */
    seq_t newM[81][81];
    for (int i = 0; i < 81; ++i)
        for (int j = 0; j < 81; ++j)
            Mpow[i][j] = M[i][j];

    /* Vector v holds the current counts of walks of the processed
     * exponent length.  Initially v_k = 1 for all k (length‑0 walks). */
    seq_t v[81], newV[81];
    for (int i = 0; i < 81; ++i) v[i] = (seq_t)1;

    idx_t exp = n;              /* exponent to apply */
    while (exp) {
        /* If the current bit is set, multiply the vector by Mpow. */
        if (exp & 1) {
            for (int i = 0; i < 81; ++i) newV[i] = 0;
            for (int i = 0; i < 81; ++i) {
                if (!v[i]) continue;
                for (int j = 0; j < 81; ++j) {
                    if (!Mpow[i][j]) continue;
                    newV[j] += v[i] * Mpow[i][j];
                }
            }
            for (int i = 0; i < 81; ++i) v[i] = newV[i];
        }

        /* Square the matrix: Mpow = Mpow * Mpow. */
        for (int i = 0; i < 81; ++i) {
            for (int j = 0; j < 81; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < 81; ++k) {
                    if (!Mpow[i][k]) continue;
                    sum += Mpow[i][k] * Mpow[k][j];
                }
                newM[i][j] = sum;
            }
        }
        for (int i = 0; i < 81; ++i)
            for (int j = 0; j < 81; ++j)
                Mpow[i][j] = newM[i][j];

        exp >>= 1;
    }

    /* Sum all end states to get the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < 81; ++i)
        total += v[i];
    return total;
}
