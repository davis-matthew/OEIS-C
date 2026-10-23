/*
 * A133647  A133566 * A000244.
 * https://oeis.org/A133647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 for n > 0: if n is odd: a(n) = 3^n if n is even: a(n) =
 *            4 * 3^(n-1) (i.e. a(n) = 4·3^(n-1)) This follows from the
 *            recurrence a(n+2) = 9·a(n) with a(0)=1, a(1)=3, a(2)=12
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fast binary exponentiation of 3^exp in seq_t type. */
static seq_t pow_seq3(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    if (n & 1)                 /* odd n */
        return pow_seq3(n);

    /* even n > 0 */
    seq_t val = pow_seq3(n - 1);
    val *= (seq_t)4;           /* multiply by 4 */
    return val;
}
