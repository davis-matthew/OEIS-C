/*
 * A068595  Number of functions from {1,2,...,n} to {1,2,...,n} such that the
 *          sum of the function values is 0 mod 3.
 * https://oeis.org/A068595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of functions from {1,2,…,n} to {1,2,…,n} such that
 *            the sum of the function values ≡ 0 (mod 3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void mat_mul3x3(const seq_t a[3][3], const seq_t b[3][3], seq_t res[3][3])
{
    for (idx_t i = 0; i < 3; ++i) {
        for (idx_t j = 0; j < 3; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 3; ++k) {
                sum = sum + a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    /* Count numbers in 1..n by residue modulo 3 */
    seq_t n_seq = (seq_t)n;
    seq_t c0 = n_seq / 3;               /* ≡0 (mod 3) */
    seq_t c1 = (n_seq + 2) / 3;         /* ≡1 (mod 3) */
    seq_t c2 = (n_seq + 1) / 3;         /* ≡2 (mod 3) */

    /* Transition matrix M (3×3) */
    seq_t M[3][3] = {
        {c0, c2, c1},
        {c1, c0, c2},
        {c2, c1, c0}
    };

    /* Result matrix = I (identity) */
    seq_t result[3][3] = {
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}
    };

    /* Binary exponentiation of M to the n‑th power */
    idx_t exp = n;
    seq_t power[3][3];
    for (idx_t i = 0; i < 3; ++i)
        for (idx_t j = 0; j < 3; ++j)
            power[i][j] = M[i][j];

    seq_t tmp[3][3];

    while (exp > 0) {
        if (exp & 1) {
            mat_mul3x3(result, power, tmp);
            for (idx_t i = 0; i < 3; ++i)
                for (idx_t j = 0; j < 3; ++j)
                    result[i][j] = tmp[i][j];
        }
        exp >>= 1;
        if (exp) {
            mat_mul3x3(power, power, tmp);
            for (idx_t i = 0; i < 3; ++i)
                for (idx_t j = 0; j < 3; ++j)
                    power[i][j] = tmp[i][j];
        }
    }

    /* Multiply result by the initial vector [1,0,0]^T;
       the desired count is the (0,0) entry of result. */
    return result[0][0];
}
