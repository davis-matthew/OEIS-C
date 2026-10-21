/*
 * A215636  a(n) = - 12*a(n-1) - 54*a(n-2) - 112*a(n-3) - 105*a(n-4) -
 *          36*a(n-5) - 2*a(n-6) with a(0)=a(1)=a(2)=0, a(3)=-3, a(4)=24,
 *          a(5)=-135.
 * https://oeis.org/A215636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -12*a(n-1) - 54*a(n-2) - 112*a(n-3) - 105*a(n-4) -
 *            36*a(n-5) - 2*a(n-6) a(0)=0, a(1)=0, a(2)=0, a(3)=-3, a(4)=24,
 *            a(5)=-135
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by binary exponentiation of the companion matrix.
   Works for all integer types assigned to seq_t and for arbitrary
   precision seq_t. */
static const seq_t coeff[6] = { -12, -54, -112, -105, -36, -2 };

seq_t a(idx_t n)
{
    /* small indices handled directly */
    if (n <= 5) {
        static const seq_t base[6] = { 0, 0, 0, -3, 24, -135 };
        return base[(int)n];
    }

    /* initial state vector [a(5), a(4), a(3), a(2), a(1), a(0)]   */
    seq_t init_vec[6] = { -135, 24, -3, 0, 0, 0 };

    /* companion matrix M (6x6) */
    seq_t M[6][6] = {0};
    for (int j = 0; j < 6; ++j) M[0][j] = coeff[j];
    for (int i = 1; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) M[i][j] = 0;
        M[i][i - 1] = 1;
    }

    /* exponent: we need M^(n-5) because state(5) is known  */
    idx_t power = n - 5;

    /* result matrix initialized to identity */
    seq_t R[6][6];
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            R[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* temporary matrix for multiplications */
    seq_t T[6][6];

    /* fast exponentiation: R = R * M^power */
    while (power) {
        if (power & 1) {
            /* R = R * M */
            for (int i = 0; i < 6; ++i) {
                for (int j = 0; j < 6; ++j) {
                    seq_t sum = 0;
                    for (int k = 0; k < 6; ++k) sum += R[i][k] * M[k][j];
                    T[i][j] = sum;
                }
            }
            for (int i = 0; i < 6; ++i)
                for (int j = 0; j < 6; ++j)
                    R[i][j] = T[i][j];
        }
        /* M = M * M */
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 6; ++j) {
                seq_t sum = 0;
                for (int k = 0; k < 6; ++k) sum += M[i][k] * M[k][j];
                T[i][j] = sum;
            }
        }
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                M[i][j] = T[i][j];

        power >>= 1;
    }

    /* a(n) is the dot product of the first row of R with init_vec */
    seq_t an = 0;
    for (int j = 0; j < 6; ++j) an += R[0][j] * init_vec[j];

    return an;
}
