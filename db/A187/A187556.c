/*
 * A187556  Triangle read by rows of products of (signless) Stirling numbers
 *          of the first kind (A132393) and Stirling numbers of the second
 *          kind (A008277).
 * https://oeis.org/A187556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) computed as the product of the signless Stirling numbers of
 *            the first kind s(n,k) and the Stirling numbers of the second
 *            kind S(n,k), where n is the row index and k the column index
 *            such that a(n)=s(n,k)*S(n,k). The triangle is read row‑by‑row,
 *            starting with a(0)=1. This implementation preco...
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 57, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define MAX_ROW 200      /* sufficient for n ≤ 10000 (triangular(200)=20100) */

static bool tables_built = false;
static seq_t s_table[MAX_ROW + 1][MAX_ROW + 1];
static seq_t S_table[MAX_ROW + 1][MAX_ROW + 1];

static void build_tables(void) {
    s_table[0][0] = 1;
    S_table[0][0] = 1;
    for (idx_t n = 1; n <= MAX_ROW; ++n) {
        s_table[n][0] = 0;
        S_table[n][0] = 0;
        for (idx_t k = 1; k <= n; ++k) {
            s_table[n][k] = s_table[n-1][k-1] + (n-1) * s_table[n-1][k];
            S_table[n][k] = S_table[n-1][k-1] + k * S_table[n-1][k];
        }
    }
    tables_built = true;
}

seq_t a(idx_t n) {
    if (!tables_built)
        build_tables();

    /* Find row i such that triangular(i) ≤ n < triangular(i+1) */
    idx_t i = 0;
    idx_t t = 0;            /* t = triangular(i) = i*(i+1)/2 */
    while (t + (i + 1) <= n) {
        ++i;
        t += i;            /* next triangular = previous + i */
    }
    idx_t j = n - t;        /* column index, 0 ≤ j ≤ i */

    /* Guard: if computed row exceeds precomputed MAX_ROW, fall back */
    if (i > MAX_ROW)
        return 0;           /* out of range for this implementation */

    return s_table[i][j] * S_table[i][j];
}
