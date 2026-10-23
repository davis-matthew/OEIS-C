/*
 * A124894  Number of base 16 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124894: Number of base‑16 circular n‑digit numbers with adjacent digits differing by at most 2.
   For n ≥ 1 the value is the trace of the n‑th power of the 16×16 adjacency matrix
   whose entries are 1 if |i−j| ≤ 2 (including the diagonal) and 0 otherwise.
   a(0) = 1. */
#include <stdbool.h>

enum { SZ = 16 };  /* fixed alphabet size */

/* matrix multiplication: res = a * b, all matrices are SZ × SZ */
static void mul_mat(const seq_t a[SZ][SZ], const seq_t b[SZ][SZ], seq_t res[SZ][SZ])
{
    for (int i = 0; i < SZ; ++i)
        for (int j = 0; j < SZ; ++j)
            res[i][j] = (seq_t)0;

    for (int i = 0; i < SZ; ++i) {
        for (int k = 0; k < SZ; ++k) {
            seq_t aik = a[i][k];
            if (aik == (seq_t)0) continue;
            for (int j = 0; j < SZ; ++j) {
                res[i][j] += aik * b[k][j];
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* adjacency matrix (initialized once) */
    static seq_t adj[SZ][SZ];
    static bool init = false;
    if (!init) {
        for (int i = 0; i < SZ; ++i) {
            for (int j = 0; j < SZ; ++j) {
                int diff = i > j ? i - j : j - i;
                adj[i][j] = (seq_t)((diff <= 2) ? 1 : 0);
            }
        }
        init = true;
    }

    /* base = adj, result = identity */
    seq_t base[SZ][SZ], result[SZ][SZ];
    for (int i = 0; i < SZ; ++i) {
        for (int j = 0; j < SZ; ++j) {
            base[i][j] = adj[i][j];
            result[i][j] = (seq_t)((i == j) ? 1 : 0);
        }
    }

    idx_t e = n;
    seq_t tmp[SZ][SZ];
    while (e > (idx_t)0) {
        if (e & (idx_t)1) {
            mul_mat(result, base, tmp);
            for (int i = 0; i < SZ; ++i)
                for (int j = 0; j < SZ; ++j)
                    result[i][j] = tmp[i][j];
        }
        mul_mat(base, base, tmp);
        for (int i = 0; i < SZ; ++i)
            for (int j = 0; j < SZ; ++j)
                base[i][j] = tmp[i][j];
        e >>= (idx_t)1;
    }

    seq_t trace = (seq_t)0;
    for (int i = 0; i < SZ; ++i)
        trace += result[i][i];
    return trace;
}
