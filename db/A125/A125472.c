/*
 * A125472  Number of base 15 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace( M^n ) for n≥1, a(0) = 1 where M is the 15×15
 *            adjacency matrix of the graph with vertices 0…14 and edges i↔j
 *            iff |i−j| ≤ 9. Thus a(n) counts the number of base‑15 circular
 *            n‑digit numbers with adjacent digits differing by at most 9
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void mat_mul(seq_t out[15][15], const seq_t A[15][15], const seq_t B[15][15])
{
    for (idx_t i = 0; i < 15; ++i) {
        for (idx_t j = 0; j < 15; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 15; ++k) {
                sum += A[i][k] * B[k][j];
            }
            out[i][j] = sum;
        }
    }
}

static void mat_copy(seq_t dst[15][15], const seq_t src[15][15])
{
    for (idx_t i = 0; i < 15; ++i)
        for (idx_t j = 0; j < 15; ++j)
            dst[i][j] = src[i][j];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* definition of a(0) in the OEIS entry */

    /* ------------------------------------------------------------------ */
    /*  Build the base matrix M and its powers M^(2^k) (k = 0…14).      */
    /* ------------------------------------------------------------------ */
    static int initialized = 0;
    static seq_t base[15][15];
    static seq_t pow_tbl[15][15][15];   /* pow_tbl[k] == M^(2^k) */

    if (!initialized) {
        /* base matrix */
        for (idx_t i = 0; i < 15; ++i) {
            for (idx_t j = 0; j < 15; ++j) {
                int diff = (i > j) ? (i - j) : (j - i);
                base[i][j] = (diff <= 9) ? 1 : 0;
            }
        }
        /* first power */
        mat_copy(pow_tbl[0], base);
        /* higher powers */
        seq_t tmp[15][15];
        for (idx_t p = 1; p < 15; ++p) {
            mat_mul(tmp, pow_tbl[p-1], pow_tbl[p-1]);
            mat_copy(pow_tbl[p], tmp);
        }
        initialized = 1;
    }

    /* ------------------------------------------------------------------ */
    /*  Exponentiation: compute M^n by binary decomposition.            */
    /* ------------------------------------------------------------------ */
    seq_t result[15][15];
    seq_t tmp[15][15];
    /* initialize result = identity */
    for (idx_t i = 0; i < 15; ++i)
        for (idx_t j = 0; j < 15; ++j)
            result[i][j] = (i == j) ? 1 : 0;

    idx_t bit = 0;
    idx_t nn = n;
    while (nn) {
        if (nn & 1) {
            /* result = result * pow_tbl[bit] */
            mat_mul(tmp, result, pow_tbl[bit]);
            mat_copy(result, tmp);
        }
        nn >>= 1;
        ++bit;
    }

    /* ------------------------------------------------------------------ */
    /*  Compute trace of M^n.                                            */
    /* ------------------------------------------------------------------ */
    seq_t total = 0;
    for (idx_t i = 0; i < 15; ++i)
        total += result[i][i];

    return total;
}
