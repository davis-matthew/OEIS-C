/*
 * A125392  Number of base 29 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n > 0; a(0) = 1. A is the 29×29 adjacency
 *            matrix with A[i][j] = 1 if |i-j| ≤ 5, else 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Build adjacency matrix */
    static seq_t adj[29][29];
    static int adj_ready = 0;
    if (!adj_ready) {
        adj_ready = 1;
        for (idx_t i = 0; i < 29; ++i) {
            for (idx_t j = 0; j < 29; ++j) {
                idx_t diff = (i > j) ? (i - j) : (j - i);
                adj[i][j] = (diff <= 5) ? (seq_t)1 : (seq_t)0;
            }
        }
    }

    /* Matrix exponentiation: compute adj^n -> res */
    seq_t res[29][29];
    seq_t power[29][29];
    seq_t tmp[29][29];

    /* Initialise res = I (identity), power = adj */
    for (idx_t i = 0; i < 29; ++i) {
        for (idx_t j = 0; j < 29; ++j) {
            res[i][j]   = (i == j) ? (seq_t)1 : (seq_t)0;
            power[i][j] = adj[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            /* res = res * power */
            for (idx_t i = 0; i < 29; ++i) {
                for (idx_t j = 0; j < 29; ++j) {
                    seq_t sum = 0;
                    for (idx_t k = 0; k < 29; ++k) {
                        sum += res[i][k] * power[k][j];
                    }
                    tmp[i][j] = sum;
                }
            }
            for (idx_t i = 0; i < 29; ++i) {
                for (idx_t j = 0; j < 29; ++j) {
                    res[i][j] = tmp[i][j];
                }
            }
        }

        /* power = power * power */
        for (idx_t i = 0; i < 29; ++i) {
            for (idx_t j = 0; j < 29; ++j) {
                seq_t sum = 0;
                for (idx_t k = 0; k < 29; ++k) {
                    sum += power[i][k] * power[k][j];
                }
                tmp[i][j] = sum;
            }
        }
        for (idx_t i = 0; i < 29; ++i) {
            for (idx_t j = 0; j < 29; ++j) {
                power[i][j] = tmp[i][j];
            }
        }

        exp >>= 1;
    }

    /* Compute trace of res */
    seq_t total = 0;
    for (idx_t i = 0; i < 29; ++i) {
        total += res[i][i];
    }
    return total;
}
