/*
 * A116156  a(n) = 5^n * n*(n + 1).
 * https://oeis.org/A116156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n * n*(n + 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case for n = 0: result is 0 */
    if (n == 0) {
        return 0;
    }

    /* Compute the polynomial part: n*(n+1) */
    seq_t nn   = (seq_t)n;        /* cast index to seq_t */
    seq_t prod = nn * (nn + 1);

    /* Compute 5^n using binary exponentiation */
    seq_t pow   = 1;
    seq_t base  = 5;
    idx_t k = n;                  /* work with a copy of n */
    while (k > 0) {
        if (k & 1) {
            pow *= base;
        }
        base *= base;
        k >>= 1;
    }

    /* Final multiplication */
    return pow * prod;
}
