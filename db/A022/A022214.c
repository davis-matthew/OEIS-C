/*
 * A022214  Gaussian binomial coefficients [ n,7 ] for q = 5.
 * https://oeis.org/A022214
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..7} (5^(n-i+1)-1)/(5^i-1), for n ≥ 7
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence [n+1,7]_5 = [n,7]_5 * (5^(n+1)-1)/(5^(n+1-7)-1) */

seq_t a(idx_t n)
{
    /* For indices below the offset the value is irrelevant. */
    if (n < 7)
        return 0;

    /* Base value at n = 7 : [7,7]_5 = 1 */
    if (n == 7)
        return 1;

    seq_t result = 1;

    /* Pre‑compute powers of 5 needed for the first step (from n = 7 → 8). */
    seq_t pow5_denom = 5;          /* 5^1  (denominator exponent starts at 1) */
    seq_t pow5_num   = pow5_denom; /* start with 5^1 , then multiply 7 times  */
    for (int i = 0; i < 7; ++i)
        pow5_num *= 5;              /* gives 5^8 for the first numerator */

    /* Iterate the recurrence from n = 7 up to n-1 */
    for (idx_t cur = 7; cur < n; ++cur) {
        seq_t numerator   = pow5_num - 1;    /* 5^(cur+1) - 1   */
        seq_t denominator = pow5_denom - 1;  /* 5^(cur+1-7) - 1 */

        /* Exact division – the result is always an integer. */
        result = result * numerator / denominator;

        /* Advance the powers of 5 for the next step. */
        pow5_num   *= 5;   /* next numerator exponent increases by 1 */
        pow5_denom *= 5;   /* next denominator exponent increases by 1 */
    }

    return result;
}
