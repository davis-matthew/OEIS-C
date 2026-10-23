/*
 * A121213  a(n) = 7^n - 5^n.
 * https://oeis.org/A121213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 5^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    Recurrence: a(n) = 12*a(n-1) - 35*a(n-2);
 *    Initial values: a(0) = 0, a(1) = 2.
 */

/* Computes seq_t value of base^exp using exponentiation by squaring.
 * Uses only integer arithmetic allowed by the constraints. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p7 = pow_seq((seq_t)7, n);
    seq_t p5 = pow_seq((seq_t)5, n);
    return p7 - p5;
}
