/*
 * A237333  Number of (n+1) X (1+1) 0..3 arrays with the maximum minus the
 *          minimum of every 2 X 2 subblock equal.
 * https://oeis.org/A237333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A237333 – number of (n+1)×2 arrays with elements 0..3 such that
    the maximum minus the minimum of every 2×2 sub‑block is constant.
    We iterate over the four possible differences d = 0,1,2,3.
    For a fixed d, let A_d be the 16×16 adjacency matrix where
        A_d[s][t] = 1  iff  max of {s,t} – min of {s,t}  equals d
    (states s,t encode two digits (x,y) as s = 4·x + y).
    Then the number of arrays of length n+1 (i.e. n transitions) with
    this fixed difference d equals the sum of all entries of A_d^n.
    The desired value is
        a(n) = Σ_{d=0}^3  Σ_{i,j} (A_d^n)[i][j]
*/

#include <stdbool.h>

#define SZ 16   /* number of possible 2‑digit states */

static void mat_mult(const seq_t a[SZ][SZ], const seq_t b[SZ][SZ], seq_t res[SZ][SZ])
{
    /* res = a * b */
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            res[i][j] = (seq_t)0;
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t k = 0; k < SZ; ++k)
            for (idx_t j = 0; j < SZ; ++j)
                res[i][j] += a[i][k] * b[k][j];
}

static void mat_pow(const seq_t base[SZ][SZ], idx_t exp, seq_t result[SZ][SZ])
{
    /* result = base^exp (exp >= 0) */
    /* identity matrix */
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            result[i][j] = (i == j ? (seq_t)1 : (seq_t)0);

    seq_t cur[SZ][SZ];
    for (idx_t i = 0; i < SZ; ++i)
        for (idx_t j = 0; j < SZ; ++j)
            cur[i][j] = base[i][j];

    while (exp > 0) {
        if (exp & 1) {
            seq_t tmp[SZ][SZ];
            mat_mult(result, cur, tmp);
            for (idx_t i = 0; i < SZ; ++i)
                for (idx_t j = 0; j < SZ; ++j)
                    result[i][j] = tmp[i][j];
        }
        exp >>= 1;
        if (exp == 0) break;
        seq_t tmp2[SZ][SZ];
        mat_mult(cur, cur, tmp2);
        for (idx_t i = 0; i < SZ; ++i)
            for (idx_t j = 0; j < SZ; ++j)
                cur[i][j] = tmp2[i][j];
    }
}

seq_t a(idx_t n)
{
    /* n >= 1, n is number of transitions */
    static bool init = false;
    static seq_t A[4][SZ][SZ];   /* adjacency matrices for d = 0..3 */

    if (!init) {  /* fill adjacency matrices once */
        for (int d = 0; d < 4; ++d) {
            for (idx_t s = 0; s < SZ; ++s) {
                int a = s / 4;
                int b = s % 4;
                for (idx_t t = 0; t < SZ; ++t) {
                    int c = t / 4;
                    int dval = t % 4;
                    int mx = a;
                    if (b > mx) mx = b;
                    if (c > mx) mx = c;
                    if (dval > mx) mx = dval;
                    int mn = a;
                    if (b < mn) mn = b;
                    if (c < mn) mn = c;
                    if (dval < mn) mn = dval;
                    int diff = mx - mn;
                    A[d][s][t] = (diff == d ? (seq_t)1 : (seq_t)0);
                }
            }
        }
        init = true;
    }

    seq_t total = (seq_t)0;

    for (int d = 0; d < 4; ++d) {
        seq_t P[SZ][SZ];
        mat_pow(A[d], n, P);   /* exponent = n transitions */

        /* sum all entries of P */
        seq_t sum = (seq_t)0;
        for (idx_t i = 0; i < SZ; ++i)
            for (idx_t j = 0; j < SZ; ++j)
                sum += P[i][j];
        total += sum;
    }

    return total;
}
