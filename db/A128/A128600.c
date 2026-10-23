/*
 * A128600  a(n) = the denominator of the continued fraction
 *          [[n/1];[n/2],[n/3],..,[n/n]], where [x] is floor(x).
 * https://oeis.org/A128600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = denominator of the continued fraction
 *            [[n/1];[n/2],[n/3],…,[n/n]] where [x] denotes ⌊x⌋. The value is
 *            computed iteratively from the end of the continued fraction
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0 the function is undefined in OEIS, but we return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Start from the last term a_{n-1} = ⌊n/n⌋ = 1. */
    seq_t num = 1;     /* numerator of the current continued fraction value   */
    seq_t den = 1;     /* denominator of the current continued fraction value   */

    /* Iterate backwards through a_{n-2}, a_{n-3}, …, a_0. */
    idx_t k = n - 1;      /* k is the index of the next term to process */
    while (k > 0) {
        --k;
        /* a_k = ⌊n / (k+1)⌋ */
        seq_t ak = (seq_t)(n / (k + 1));

        /* Update: new_num = a_k * num + den, new_den = num */
        seq_t temp = num;
        num = ak * num + den;
        den = temp;
    }

    return den;   /* denominator of the final fraction */
}
