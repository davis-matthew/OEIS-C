/*
 * A125436  Number of base 24 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A125436 – Number of base‑24 circular n‑digit numbers with neighbouring
    digits differing by at most 7.
    We model the digits as vertices 0…23 of a graph with an edge between
    i and j iff |i–j| ≤ 7.  For an n‑tuple (d0 … dn‑1) to be a
    circular number, every consecutive pair must be an edge, and the last
    and the first digits must also be adjacent.  The number of such
    n‑tuples is the number of closed walks of length n in that graph,
    which equals the trace of the n‑th power of its adjacency matrix.
    Hence

        a(n) = tr( A^n ),  A[i][j] = 1   if |i-j| ≤ 7
                          0   otherwise
    with a(0)=1 by convention. */

#include <stddef.h>
#include <stdbool.h>        /* for bool */

#define B 24                  /* size of the base */

static void init_adj_matrix(seq_t adj[B][B])
{
    for (int i = 0; i < B; ++i) {
        for (int j = 0; j < B; ++j) {
            int diff = i - j;
            if (diff < 0) diff = -diff;
            adj[i][j] = (diff <= 7) ? 1 : 0;
        }
    }
}

static void mat_mul(const seq_t a[B][B], const seq_t b[B][B], seq_t res[B][B])
{
    for (int i = 0; i < B; ++i) {
        for (int j = 0; j < B; ++j) {
            seq_t s = 0;
            for (int k = 0; k < B; ++k) {
                s += a[i][k] * b[k][j];
            }
            res[i][j] = s;
        }
    }
}

static void mat_copy(seq_t dest[B][B], const seq_t src[B][B])
{
    for (int i = 0; i < B; ++i) {
        for (int j = 0; j < B; ++j) {
            dest[i][j] = src[i][j];
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    static bool built = false;
    static seq_t base[B][B];
    if (!built) {
        init_adj_matrix(base);
        built = true;
    }

    /* Result initialized to the identity matrix. */
    seq_t res[B][B] = {0};
    for (int i = 0; i < B; ++i) res[i][i] = 1;

    /* Current power of the base matrix. */
    seq_t power[B][B];
    mat_copy(power, base);

    idx_t exp = n;
    seq_t tmp[B][B];

    while (exp) {
        if (exp & 1) {
            mat_mul(res, power, tmp);
            mat_copy(res, tmp);
        }
        exp >>= 1;
        if (exp) {
            mat_mul(power, power, tmp);
            mat_copy(power, tmp);
        }
    }

    /* Sum of the diagonal elements gives the trace. */
    seq_t sum = 0;
    for (int i = 0; i < B; ++i) sum += res[i][i];
    return sum;
}
