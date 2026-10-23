/*
 * A125379  Number of base 16 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1, with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A is the 16×16 adjacency matrix of the graph on {0,…,15} where   */
/*  A[i][j] = 1 iff |i‑j| ≤ 5, otherwise A[i][j] = 0.            */
seq_t a(idx_t n)
{
    /*  a(0) is defined to be 1 in the OEIS entry.  */
    if (n == 0) return 1;

    /*  Static adjacency matrix that is computed once.  */
    static seq_t Ad[16][16];
    static int Ad_initialized = 0;

    if (!Ad_initialized) {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                seq_t diff = (i > j) ? (seq_t)(i - j) : (seq_t)(j - i);
                Ad[i][j] = (diff <= 5) ? 1 : 0;
            }
        }
        Ad_initialized = 1;
    }

    /*  Local copies of matrices used in exponentiation.  */
    seq_t res[16][16];
    seq_t base[16][16];
    seq_t temp[16][16];

    /*  res = I (identity matrix).  */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            res[i][j] = (i == j) ? 1 : 0;

    /*  base = Ad (copy of the adjacency matrix).  */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            base[i][j] = Ad[i][j];

    /*  Binary exponentiation of the matrix:  res = Ad^n.  */
    idx_t power = n;
    while (power > 0) {
        if (power & 1) {
            /*  res = res * base  */
            for (int i = 0; i < 16; ++i) {
                for (int j = 0; j < 16; ++j) {
                    seq_t sum = 0;
                    for (int k = 0; k < 16; ++k)
                        sum += res[i][k] * base[k][j];
                    temp[i][j] = sum;
                }
            }
            for (int i = 0; i < 16; ++i)
                for (int j = 0; j < 16; ++j)
                    res[i][j] = temp[i][j];
        }

        /*  base = base * base  */
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < 16; ++k)
                    sum += base[i][k] * base[k][j];
                temp[i][j] = sum;
            }
        }
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                base[i][j] = temp[i][j];

        power >>= 1;
    }

    /*  Compute the trace of the resulting matrix.  */
    seq_t trace = 0;
    for (int i = 0; i < 16; ++i)
        trace += res[i][i];

    return trace;
}
