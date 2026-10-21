/*
 * A055599  Triangle T(n,k) read by rows, giving the number of n X n binary
 *          matrices with no zero rows or columns and with k=0..n^2 ones.
 * https://oeis.org/A055599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) gives the flattened triangle of A055599. The nth term is
 *            the (n‑th) entry in the triangle read row by row: T(n,k) =
 *            sum_{i=0..n} (-1)^i * C(n,i) * sum_{j=0..n} (-1)^j * C(n,j) *
 *            C((n−j)*n, k)
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

/* Computes a(m,n,k) from the OEIS definition. */
static seq_t
a_mn_k(idx_t m, idx_t n, idx_t k)
{
    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t j = 0; j <= m; ++j) {
        seq_t bin1 = binomial(m, j);
        if (bin1 == 0) continue;

        /* cells in the (m-j)×n matrix. */
        idx_t cells = (m - j) * n;
        seq_t bin2 = binomial(cells, k);
        seq_t term = bin1 * bin2;

        if ((j & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;  /* pos ≥ neg for all inputs. */
}

/* Computes T(n,k) for a fixed row n and column k. */
static seq_t
T_row(idx_t n, idx_t k)
{
    seq_t pos = 0;
    seq_t neg = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t bin_i = binomial(n, i);
        seq_t sub = a_mn_k(n, n - i, k);
        seq_t term = bin_i * sub;

        if ((i & 1) == 0)
            pos += term;
        else
            neg += term;
    }
    return pos - neg;  /* pos ≥ neg for all inputs. */
}

/* External entry point: a(idx_t n) returns the nth term of the flattened
   triangle of binary matrices with no zero rows or columns. */
seq_t
a(idx_t n)
{
    /* Find the row r such that the nth term lies in that row. */
    idx_t cumulative = 0;
    idx_t r = 1;
    while (1) {
        /* Number of terms in row r: r^2 + 1 */
        idx_t terms = r * r + 1;
        if (n <= cumulative + terms)
            break;
        cumulative += terms;
        ++r;
    }

    /* Column index within the row (0‑based). */
    idx_t k = n - cumulative - 1;
    return T_row(r, k);
}
