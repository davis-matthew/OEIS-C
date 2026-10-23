/*
 * A181889  Bisection of A181888.
 * https://oeis.org/A181889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n-1)*(2*n+1)*(2*n-1)*(5*n+6)*(n+1)/45
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1657,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = 2 * n * (n-1) * (2n+1) * (2n-1) * (5n+6) * (n+1) / 45  */
/* The routine cancels the divisor 45 as early as possible to keep       */
/* intermediate values in the range of seq_t (which is at most 64 bits  */
/* for the 64‑bit build).                                                */

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1 but guard anyway */
        return (seq_t)0;

    seq_t N = (seq_t)n;
    seq_t product = (seq_t)1;

    /* factors of the numerator (the 7 factors) */
    seq_t fs[7];
    fs[0] = (seq_t)2;         /* the leading 2 */
    fs[1] = N;
    fs[2] = N - (seq_t)1;
    fs[3] = (seq_t)2 * N + (seq_t)1;
    fs[4] = (seq_t)2 * N - (seq_t)1;
    fs[5] = (seq_t)5 * N + (seq_t)6;
    fs[6] = N + (seq_t)1;

    /* remaining factors of the divisor 45 = 3 * 3 * 5 */
    int rem[3] = {3, 3, 5};
    int rem_cnt = 3;

    for (int i = 0; i < 7; ++i) {
        seq_t f = fs[i];

        /* cancel factor f with any remaining parts of 45 */
        for (int j = 0; j < rem_cnt; ++j) {
            if (f % rem[j] == 0) {
                f /= rem[j];
                /* shift remaining factors left to remove used one */
                for (int k = j; k < rem_cnt - 1; ++k)
                    rem[k] = rem[k + 1];
                --rem_cnt;
                --j;    /* re-check the current position */
            }
        }

        /* multiply the reduced factor into the running product */
        product *= f;

        /* try to cancel product itself with any remaining parts of 45 */
        for (int j = 0; j < rem_cnt; ++j) {
            if (product % rem[j] == 0) {
                product /= rem[j];
                for (int k = j; k < rem_cnt - 1; ++k)
                    rem[k] = rem[k + 1];
                --rem_cnt;
                --j;
            }
        }
    }

    /* after processing all factors, rem_cnt should be 0 */
    return product;
}
