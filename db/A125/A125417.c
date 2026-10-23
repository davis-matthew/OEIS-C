/*
 * A125417  Number of base 29 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| ≤ 6 else 0, a(0) =
 *            1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void mat_mul(seq_t res[29][29], const seq_t a[29][29], const seq_t b[29][29])
{
    seq_t tmp[29][29];
    for (int i = 0; i < 29; ++i)
        for (int j = 0; j < 29; ++j)
            tmp[i][j] = 0;

    for (int i = 0; i < 29; ++i) {
        for (int k = 0; k < 29; ++k) {
            seq_t aik = a[i][k];
            if (!aik) continue;
            for (int j = 0; j < 29; ++j) {
                tmp[i][j] += aik * b[k][j];
            }
        }
    }

    for (int i = 0; i < 29; ++i)
        for (int j = 0; j < 29; ++j)
            res[i][j] = tmp[i][j];
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* base case */

    /* Static adjacency matrix A (29 × 29) */
    static int initialized = 0;
    static seq_t Ad[29][29];
    if (!initialized) {
        for (int i = 0; i < 29; ++i) {
            for (int j = 0; j < 29; ++j) {
                int diff = i > j ? i - j : j - i;
                Ad[i][j] = (diff <= 6) ? 1 : 0;
            }
        }
        initialized = 1;
    }

    /* Res = I (identity matrix) */
    seq_t res[29][29];
    for (int i = 0; i < 29; ++i)
        for (int j = 0; j < 29; ++j)
            res[i][j] = (i == j) ? 1 : 0;

    seq_t base[29][29];
    for (int i = 0; i < 29; ++i)
        for (int j = 0; j < 29; ++j)
            base[i][j] = Ad[i][j];

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            mat_mul(res, res, base);
        }
        mat_mul(base, base, base);
        exp >>= 1;
    }

    /* trace(res) = desired count */
    seq_t tr = 0;
    for (int i = 0; i < 29; ++i)
        tr += res[i][i];

    return tr;
}
