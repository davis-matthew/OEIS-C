/*
 * A108681  a(n) = (n+1)*(n+2)^2*(n+3)*(n+4)*(n+5)*(2*n+3)/720.
 * https://oeis.org/A108681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)^2*(n+3)*(n+4)*(n+5)*(2*n+3)/720
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Denominator 720 factors: 2^4 * 3^2 * 5 */
    int den[7] = {2, 2, 2, 2, 3, 3, 5};
    const int dcnt = 7;

    /* Numerator factors */
    const int num_factors[7] = {
        (int)(n + 1),
        (int)(n + 2),
        (int)(n + 2),
        (int)(n + 3),
        (int)(n + 4),
        (int)(n + 5),
        (int)(2 * n + 3)
    };

    seq_t res = 1;  /* intermediate result */

    for (int i = 0; i < 7; ++i) {
        seq_t tf = (seq_t)num_factors[i];

        /* Reduce the numerator factor with unused denominator factors */
        for (int j = 0; j < dcnt; ++j) {
            if (den[j] == 0) continue;   /* already used */
            if ((tf % den[j]) == 0) {
                tf /= den[j];
                den[j] = 0;               /* mark as used */
            }
        }

        /* Multiply reduced factor into the result */
        res *= tf;

        /* Try to cancel remaining unused denominator factors from the result */
        for (int j = 0; j < dcnt; ++j) {
            if (den[j] == 0) continue;
            if ((res % den[j]) == 0) {
                res /= den[j];
                den[j] = 0;
            }
        }
    }

    /* Any denominator factors not used yet must divide the current result */
    for (int j = 0; j < dcnt; ++j) {
        if (den[j] != 0) {
            res /= den[j];
        }
    }

    return res;
}
