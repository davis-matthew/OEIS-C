/*
 * A093357  Number of occurrences of pattern 2-1 after n iterations of
 *          morphism A007413.
 * https://oeis.org/A093357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*4^(n-1) - 2^n)/2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t power_of_2(idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= 2;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)            /* protection for 0‑based callers */
        return 0;

    idx_t exp4 = 2 * (n - 1);   /* exponent for 4^(n‑1) = 2^(2*(n‑1)) */
    seq_t pow4 = power_of_2(exp4);   /* 4^(n‑1) */
    seq_t pow2n = power_of_2(n);     /* 2^n */

    seq_t temp = pow4 * 3;  /* 3 * 4^(n‑1) */
    temp = temp - pow2n;    /* minus 2^n */
    temp = temp / 2;        /* divide by 2, truncates as needed */

    return temp;
}
