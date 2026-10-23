/*
 * A154879  Third differences of the Jacobsthal sequence A001045.
 * https://oeis.org/A154879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2), a(0) = 3, a(1) = -2
 * Range:     i8 n <= 8, i16 n <= 16, i32 n <= 32, i64 n <= 64, i128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0u) return (seq_t)3;
    if (n == 1u) return (seq_t)-2;

    /* Companion matrix for the recurrence: M = [[1,2],[1,0]] */
    struct { seq_t a00, a01, a10, a11; } M = { (seq_t)1, (seq_t)2,
                                                 (seq_t)1, (seq_t)0 };
    /* Identity matrix for binary exponentiation */
    struct { seq_t a00, a01, a10, a11; } res = { (seq_t)1, (seq_t)0,
                                                   (seq_t)0, (seq_t)1 };

    idx_t power = n - 1u;

    /* Binary exponentiation: compute M^(n-1) into res */
    while (power > 0u) {
        if (power & 1u) {
            /* res = res * M */
            seq_t t00 = res.a00 * M.a00 + res.a01 * M.a10;
            seq_t t01 = res.a00 * M.a01 + res.a01 * M.a11;
            seq_t t10 = res.a10 * M.a00 + res.a11 * M.a10;
            seq_t t11 = res.a10 * M.a01 + res.a11 * M.a11;
            res.a00 = t00; res.a01 = t01; res.a10 = t10; res.a11 = t11;
        }
        /* M = M * M */
        seq_t t00 = M.a00 * M.a00 + M.a01 * M.a10;
        seq_t t01 = M.a00 * M.a01 + M.a01 * M.a11;
        seq_t t10 = M.a10 * M.a00 + M.a11 * M.a10;
        seq_t t11 = M.a10 * M.a01 + M.a11 * M.a11;
        M.a00 = t00; M.a01 = t01; M.a10 = t10; M.a11 = t11;

        power >>= 1u;
    }

    /* a(n) = res.a00 * a1 + res.a01 * a0 */
    seq_t a0 = (seq_t)3;
    seq_t a1 = (seq_t)-2;
    return res.a00 * a1 + res.a01 * a0;
}
