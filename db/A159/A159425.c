/*
 * A159425  Number of n X n arrays of squares of integers summing to 30.
 * https://oeis.org/A159425
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^30] (1 + x + x^4 + x^9 + x^16 + x^25)^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_DEG 30

seq_t a(idx_t n)
{
    /* polynomial representing one cell: 1 + x + x^4 + x^9 + x^16 + x^25 */
    seq_t base[MAX_DEG + 1] = {0};
    seq_t result[MAX_DEG + 1] = {0};
    seq_t tmp[MAX_DEG + 1]    = {0};

    base[0] = 1;
    base[1] = 1;
    base[4] = 1;
    base[9] = 1;
    base[16] = 1;
    base[25] = 1;

    /* a(0) and a(1) are irrelevant; the sequence only starts at n = 2 */
    if (n < 2)
        return 0;

    /* exponent is n^2 (number of cells in the array) */
    idx_t exp = n * n;

    /* result = base^0 = 1 */
    result[0] = 1;

    /* Binary exponentiation of polynomials, keeping only terms up to degree 30 */
    while (exp)
    {
        if (exp & 1ULL)   /* result *= base */
        {
            for (int i = 0; i <= MAX_DEG; ++i)
                tmp[i] = 0;

            for (int i = 0; i <= MAX_DEG; ++i)
            {
                if (!result[i]) continue;
                for (int j = 0; j <= MAX_DEG - i; ++j)
                {
                    if (!base[j]) continue;
                    tmp[i + j] += result[i] * base[j];
                }
            }

            for (int i = 0; i <= MAX_DEG; ++i)
                result[i] = tmp[i];
        }

        /* base *= base */
        for (int i = 0; i <= MAX_DEG; ++i)
            tmp[i] = 0;

        for (int i = 0; i <= MAX_DEG; ++i)
        {
            if (!base[i]) continue;
            for (int j = 0; j <= MAX_DEG - i; ++j)
            {
                if (!base[j]) continue;
                tmp[i + j] += base[i] * base[j];
            }
        }

        for (int i = 0; i <= MAX_DEG; ++i)
            base[i] = tmp[i];

        exp >>= 1ULL;
    }

    return result[MAX_DEG];   /* coefficient of x^30 */
}
