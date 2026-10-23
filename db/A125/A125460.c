/*
 * A125460  Number of base 25 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n > 0 and a(0) = 1, where A[i][j] = 1 iff
 *            |i-j| ≤ 8 (self‑loops included)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void mat_mul(const seq_t *A, const seq_t *B, seq_t *C)
{
    for (idx_t i = 0; i < 25; ++i) {
        for (idx_t j = 0; j < 25; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 25; ++k)
                sum += A[i * 25 + k] * B[k * 25 + j];
            C[i * 25 + j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Build adjacency matrix once. */
    static seq_t adj[625];
    static int adj_init = 0;
    if (!adj_init) {
        for (idx_t i = 0; i < 25; ++i) {
            for (idx_t j = 0; j < 25; ++j) {
                int diff = (i > j) ? (i - j) : (j - i);
                adj[i * 25 + j] = (diff <= 8) ? (seq_t)1 : (seq_t)0;
            }
        }
        adj_init = 1;
    }

    /* Identity matrix. */
    seq_t res[625];
    seq_t cur[625];
    for (idx_t i = 0; i < 25; ++i) {
        for (idx_t j = 0; j < 25; ++j) {
            res[i * 25 + j] = (i == j) ? (seq_t)1 : (seq_t)0;
            cur[i * 25 + j] = adj[i * 25 + j];
        }
    }

    idx_t exp = n;
    seq_t tmp[625];

    /* Binary exponentiation: res = I, cur = A. */
    while (exp > 0) {
        if (exp & 1) {          /* res = res * cur */
            mat_mul(res, cur, tmp);
            for (idx_t i = 0; i < 625; ++i)
                res[i] = tmp[i];
        }
        /* cur = cur * cur */
        mat_mul(cur, cur, tmp);
        for (idx_t i = 0; i < 625; ++i)
            cur[i] = tmp[i];

        exp >>= 1;
    }

    /* Trace of the resulting matrix. */
    seq_t result = 0;
    for (idx_t i = 0; i < 25; ++i)
        result += res[i * 25 + i];

    return result;
}
