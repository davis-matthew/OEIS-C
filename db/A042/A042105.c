/*
 * A042105  Denominators of continued fraction convergents to sqrt(577).
 * https://oeis.org/A042105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 48*a(n-1) + a(n-2), a(0)=1, a(1)=48
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static void mat_mul(const seq_t a[2][2], const seq_t b[2][2], seq_t out[2][2])
{
    out[0][0] = a[0][0] * b[0][0] + a[0][1] * b[1][0];
    out[0][1] = a[0][0] * b[0][1] + a[0][1] * b[1][1];
    out[1][0] = a[1][0] * b[0][0] + a[1][1] * b[1][0];
    out[1][1] = a[1][0] * b[0][1] + a[1][1] * b[1][1];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* a(0) */

    /* Base matrix for the recurrence: [[48, 1], [1, 0]] */
    seq_t base[2][2] = { { 48, 1 }, { 1, 0 } };
    /* Result matrix (initially the identity) */
    seq_t res[2][2]  = { { 1, 0 }, { 0, 1 } };

    idx_t k = n - 1;   /* We need M^(n-1) */

    while (k)
    {
        if (k & 1)
        {
            seq_t tmp[2][2];
            mat_mul(res, base, tmp);
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    res[i][j] = tmp[i][j];
        }

        /* Square the base matrix: base = base * base */
        {
            seq_t tmp[2][2];
            mat_mul(base, base, tmp);
            for (int i = 0; i < 2; ++i)
                for (int j = 0; j < 2; ++j)
                    base[i][j] = tmp[i][j];
        }

        k >>= 1;
    }

    /* Apply the matrix to the initial vector [a(1), a(0)] = [48, 1] */
    return res[0][0] * 48 + res[0][1];
}
