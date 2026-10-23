/*
 * A106732  Expansion of -3*x/(1 - 5*x + 3*x^2).
 * https://oeis.org/A106732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(0) = 0, a(1) = -3; a(n) = 5*a(n-1) - 3*a(n-2) for n ≥ 2
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 30, i128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)-3;

    /* Fast binary exponentiation of the recurrence matrix */
    seq_t M[2][2] = { { (seq_t)5,  (seq_t)-3 },
                      { (seq_t)1,  (seq_t)0 } };

    seq_t R[2][2] = { { (seq_t)1,  (seq_t)0 },
                      { (seq_t)0,  (seq_t)1 } };

    idx_t exp = n - 1;
    seq_t tmp[2][2];

    while (exp) {
        if (exp & (idx_t)1) {
            /* R = R * M */
            tmp[0][0] = R[0][0]*M[0][0] + R[0][1]*M[1][0];
            tmp[0][1] = R[0][0]*M[0][1] + R[0][1]*M[1][1];
            tmp[1][0] = R[1][0]*M[0][0] + R[1][1]*M[1][0];
            tmp[1][1] = R[1][0]*M[0][1] + R[1][1]*M[1][1];
            R[0][0] = tmp[0][0]; R[0][1] = tmp[0][1];
            R[1][0] = tmp[1][0]; R[1][1] = tmp[1][1];
        }

        /* M = M * M */
        tmp[0][0] = M[0][0]*M[0][0] + M[0][1]*M[1][0];
        tmp[0][1] = M[0][0]*M[0][1] + M[0][1]*M[1][1];
        tmp[1][0] = M[1][0]*M[0][0] + M[1][1]*M[1][0];
        tmp[1][1] = M[1][0]*M[0][1] + M[1][1]*M[1][1];
        M[0][0] = tmp[0][0]; M[0][1] = tmp[0][1];
        M[1][0] = tmp[1][0]; M[1][1] = tmp[1][1];

        exp >>= 1;
    }

    /* a(n) = -3 * R[0][0] */
    seq_t result = R[0][0];
    result = result * (seq_t)(-3);
    return result;
}
