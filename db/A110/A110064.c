/*
 * A110064  a(n+4) = a(n+1) - a(n), a(0) = 1, a(1) = -4, a(2) = 0, a(3) = 1.
 * https://oeis.org/A110064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n+4) = a(n+1) – a(n) * a(0)=1, a(1)=-4, a(2)=0, a(3)=1 * A
 *            linear‑recurrence of order 4 is solved by matrix exponentiation,
 *            * giving O(log n) time and O(1) memory, suitable for both *
 *            fixed‑size integer and arbitrary‑precision implementations
 * Range:     i8 n <= 24, i16 n <= 58, i32 n <= 58, i64 n <= 58, i128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-4;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    /* Transition matrix M: next state = M * current state */
    seq_t M[4][4] = {
        { (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)0 },
        { (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)0 },
        { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1 },
        { (seq_t)-1,(seq_t)1, (seq_t)0, (seq_t)0 }
    };

    /* Identity matrix */
    seq_t result[4][4] = {
        { (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0 },
        { (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)0 },
        { (seq_t)0, (seq_t)0, (seq_t)1, (seq_t)0 },
        { (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)1 }
    };

    /* Copy of M for repeated squaring */
    seq_t power[4][4];
    for (idx_t i = 0; i < 4; ++i)
        for (idx_t j = 0; j < 4; ++j)
            power[i][j] = M[i][j];

    /* Exponentiation by squaring: compute M^n */
    idx_t e = n;
    while (e > 0) {
        if (e & (idx_t)1) {
            /* result = result * power */
            seq_t tmp[4][4];
            for (idx_t i = 0; i < 4; ++i) {
                for (idx_t j = 0; j < 4; ++j) {
                    seq_t sum = 0;
                    for (idx_t k = 0; k < 4; ++k)
                        sum += result[i][k] * power[k][j];
                    tmp[i][j] = sum;
                }
            }
            for (idx_t i = 0; i < 4; ++i)
                for (idx_t j = 0; j < 4; ++j)
                    result[i][j] = tmp[i][j];
        }

        /* power = power * power */
        seq_t tmpPow[4][4];
        for (idx_t i = 0; i < 4; ++i) {
            for (idx_t j = 0; j < 4; ++j) {
                seq_t sum = 0;
                for (idx_t k = 0; k < 4; ++k)
                    sum += power[i][k] * power[k][j];
                tmpPow[i][j] = sum;
            }
        }
        for (idx_t i = 0; i < 4; ++i)
            for (idx_t j = 0; j < 4; ++j)
                power[i][j] = tmpPow[i][j];
        e >>= 1;
    }

    /* Base vector [a0, a1, a2, a3] */
    seq_t baseVec[4] = { (seq_t)1, (seq_t)-4, (seq_t)0, (seq_t)1 };

    /* Return the first component of result * baseVec */
    seq_t value = 0;
    for (idx_t j = 0; j < 4; ++j)
        value += result[0][j] * baseVec[j];

    return value;
}
